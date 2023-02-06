# for rabbitmq use
from typing import List, Tuple
import pika
# for socket use
import socket, os
# thread for using this server as multi-client available
import _thread
# for converting json string into proper json
import json
# for mongodb
from pymongo import MongoClient


class RmqHandler:
    def __init__(self):
        self.rabbitmq_host = os.getenv('RMQ_HOST')
        self.queue_names = os.getenv('QUEUE_NAMES').split(',')
        self.rmq_connection = pika.BlockingConnection(pika.ConnectionParameters(self.rabbitmq_host))
        self.channel = self.rmq_connection.channel()
        for queue_name in self.queue_names:
            self.channel.queue_declare(queue=queue_name)

    def send(self, msg_exchange: str, msg_routing_key: str, msg_body: str):
        self.channel.basic_publish(exchange=msg_exchange,
            routing_key=msg_routing_key,
            body=msg_body)

class MongodbHandler:
    def __init__(self):
        self.user = os.getenv('MONGO_INITDB_ROOT_USERNAME')
        self.password = os.getenv('MONGO_INITDB_ROOT_PASSWORD')
        self.db_name = os.getenv('DB_NAME')
        self.port = '27017'
        self.connection = MongoClient(f'mongodb://{self.user}:{self.password}@mongodb:{self.port}/')
        self.db = self.connection[self.db_name]
        self.connection[self.db_name]

    def is_collection_exist(self, collection_name: str) -> tuple:
        if collection_name in self.db.list_collection_names():
            return True, self.db[collection_name]
        return False, None

    def get_collection(self, collection_name: str) -> object:
        is_exist, collection = self.is_collection_exist(collection_name)
        if is_exist:
            return collection
        return None

    def get_one_filtered_with_fields(self, collection_name: str, query: dict, fields: list):
        json_object = None
        fields_to_show = {field: 1 for field in fields}
        if '_id' not in fields:
            fields_to_show['_id'] = 0
        results = self.get_collection(collection_name).find_one(query, fields_to_show)
        if results is not None:
            json_object = {key: value for key, value in results.items()}
        return json_object

    def insert_document(self, collection_name: str, document: dict) -> str:
        collection = self.db[collection_name]
        uid = collection.insert_one(document)
        print(f'\n\n(info - rmq-mdb-service) Inserted:\n{uid}\n{uid.inserted_id}\n\n')
        return str(uid.inserted_id)

    def delete_document(self, collection_name: str, document_id: str):
        collection = self.db[collection_name]
        collection.delete_one({'_id': document_id})


def on_new_client(conn, addr, buffer_size, mdb_handler, rmq_handler):
    print(f'\n\n(info - rmq-mdb-service) Got new client:\n{addr}\n\n')
    stream = ''
    while True:
        msg = conn.recv(buffer_size)
        if not msg:
            break
        # adding the new stream of messages) to the last one
        stream = stream + msg.decode()
        print(f'\n\n(info - rmq-mdb-service) Just got the stream, dividing it:\n{stream}\n\n')
        json_msgs, stream = extract_json_objects(stream=stream)
        for json_msg in json_msgs:
            print(f'\n\n(info - rmq-mdb-service) handling the next message:\n{json_msg}\n\n')
            handle_message(json_msg, mdb_handler, rmq_handler)
    print(f'\n\n(info - rmq-mdb-service) Connection close\n\n')
    conn.close()

def extract_json_objects(stream: str) -> Tuple[List[dict], str]:
    sections = []
    while len(stream) > 0:
        size_end = stream.find("{")
        if size_end == -1:
            break
        size = int(stream[:size_end])
        brace_count = 1
        json_end = size_end + 1
        while brace_count > 0 and json_end < len(stream):
            if stream[json_end] == "{":
                brace_count += 1
            elif stream[json_end] == "}":
                brace_count -= 1
            json_end += 1
        if json_end == len(stream):
            break
        json_str = stream[size_end:json_end]
        print(f'\n\njson str is getting loaded:\n{json_str}\n\n')
        sections.append(json.loads(json_str))
        stream = stream[json_end + 1:]
    return sections, stream

def handle_message(json_msg: dict, mdb_handler: MongodbHandler, rmq_handler: RmqHandler):
    print(f'\n\n(info - rmq-mdb-service) Got new message from dlep:\n{json_msg}\n\n')
    if json_msg['Stage'] == 'Progression' and json_msg['MessageType'] == 'Peer_Offer':
        print(f'\n\n(info - rmq-mdb-service) At Peer Offer before:\n{json_msg}\n\n')
        new_device(json_msg=json_msg, mdb_handler=mdb_handler, rmq_handler=rmq_handler)
        print(f'\n\n(info - rmq-mdb-service) At Peer Offer after:\n{json_msg}\n\n')
    if json_msg['Stage'] == 'Progression' and json_msg['MessageType'] == 'Peer_down':
        device_down(ip=json_msg['ModemAddress'], mdb_handler=mdb_handler, rmq_handler=rmq_handler)
    if json_msg['Direction'] == 'MtR':
        # in case of modem sending the message, adding the device_id:
        device_id = get_id_by_ip(ip=json_msg['ModemAddress'], mdb_handler=mdb_handler)
        json_msg['DeviceId'] = device_id
    insert_msg_to_db(json_msg=json_msg, mdb_handler=mdb_handler)

def new_device(json_msg: dict, mdb_handler: MongodbHandler, rmq_handler: RmqHandler):
    '''handles new device situation, MongoDBwise and RMQwise'''
    '''the json msg we are handle here is Peer_Offer from modem'''
    device_document = {}
    # first lets get the modem ip from message
    device_document['ip'] = json_msg['ModemAddress']
    # next we'll get the data items, aiming to find the 'Peer_Type' data item
    data_items = json_msg['DataItems']
    # here will be the array of peer type: '0;net_type;modem;serial_number;key'
    peer_type_item = None
    for item in data_items:
        # looking for the data item named 'Peer_Type'
        if item['Name'] == 'Peer_Type':
            peer_type_item = item['Value'].split(';')
            break
    # now that we've got the peer type data item, we can configure the device fields:
    device_document['networkType'] = peer_type_item[1]
    device_document['radioType'] = peer_type_item[2]
    device_document['serialNumber'] = peer_type_item[3]
    device_document['key'] = peer_type_item[4]
    # next we push the device to mongodb, taking the device_id as result
    print(f'\n\n(info - rmq-mdb-service) Right before Inserting device:\n{json_msg}\n\n')
    device_id = mdb_handler.insert_document('Devices', device_document)
    print(f'\n\n(info - rmq-mdb-service) Insert to mdb new device:\n{device_document}\n\n')
    # next we send the device_id over the rmq
    rmq_handler.send('', 'device_up', device_id)
    print(f'\n\n(info - rmq-mdb-service) Sending device_up over rmq:\n{device_id}\n\n')
    
def insert_msg_to_db(json_msg: dict, mdb_handler: MongodbHandler):
    '''inserts the given json message to mongodb'''
    print(f'\n\n(info - rmq-mdb-service) Right before inserting a message to db:\n{json_msg}\n\n')
    mdb_handler.insert_document('DlepMessage', json_msg)
    print(f'\n\n(info - rmq-mdb-service) Insert to mdb new dldep message:\n{json_msg}\n\n')

def device_down(ip: str, mdb_handler: MongodbHandler, rmq_handler: RmqHandler):
    '''handles device down, sends over rmq on device_down queue the device_id'''
    '''the json msg we are handle here is Peer_Down from modem'''
    # getting the device id by the given ip
    device_id = get_id_by_ip(ip=ip, mdb_handler=mdb_handler)
    # then we will delete the device from 'Devices' colleciton
    mdb_handler.delete_document('Devices', device_id)
    print(f'\n\n(info - rmq-mdb-service) Delete from mdb device:\n{device_id}\n\n')
    # now we will send the device_id over the queue 'device_down'
    rmq_handler.send('', 'device_down', device_id)
    print(f'\n\n(info - rmq-mdb-service) Sending device_down over rmq:\n{device_id}\n\n')


def get_id_by_ip(ip: str, mdb_handler: MongodbHandler):
    '''checks on the db and returns the device_id by the given ip'''
    device_id = mdb_handler.get_one_filtered_with_fields(collection_name='Devices',
        query={'ip': ip}, 
        fields={'_id'})
    return device_id


def main_func():
    print('im at the start of rmq-mdb-service')
    # tcp server configuration
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
    host = '127.0.0.1'
    port = 12345
    buffer_size = 4096
    sock.bind((host, port))
    sock.listen(5)
    # rabbitmq configuration
    rmq_handler = RmqHandler()
    # mongodb configuration
    mdb_handler = MongodbHandler()

    while True:
        # Accepting connection from client
        conn, addr = sock.accept()
        _thread.start_new_thread(on_new_client, (conn, addr, buffer_size, mdb_handler, rmq_handler))
    sock.close()
    rmq_connection.close()

if __name__ == '__main__':
    main_func()


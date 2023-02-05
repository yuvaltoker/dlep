# for rabbitmq use
import pika
# for socket use
import socket, os
# thread for using this server as multi-client available
import _thread
# for converting json string into proper json
import json
# for mongodb
from pymongo import MongoClient

class MongodbHandler:
    def __init__(self):
        # Get environment variables
        self.user = os.getenv('MONGO_INITDB_ROOT_USERNAME')
        self.password = os.getenv('MONGO_INITDB_ROOT_PASSWORD')
        self.db_name = os.getenv('DB_NAME')
        self.port = '27017'
        #self.connection = MongoClient('mongodb://root:example@mongodb:27017/')
        self.connection = MongoClient('mongodb://%s:%s@%s:%s/' %(self.user, self.password, 'mongodb', self.port))
        self.db = self.connection[self.db_name]
        # creating db
        self.connection[self.db_name]

    def is_collection_exist(self, collection_name):
        if collection_name in self.db.list_collection_names():
            return True, self.db[collection_name]
        return False, None
    
    def get_collection(self, collection_name):
        is_exist, collection = self.is_collection_exist(collection_name)
        if is_exist:
            return collection
        return None

    # returns specific fields of document filtered by query {field : value, ...}, currently _id will always be shown
    # note that when an empty list is given in fields, all fields are going to be in the returning dictionary
    def get_one_filtered_with_fields(self, collection_name, query, fields):
        json_object = None
        # create the next dict: {field1 : 1, field2 : 1, field3 : 1,... fieldN : 1}
        # which means, show each of the fields in fields
        fields_to_show = {field : 1 for field in fields}
        # for some reason, as a default, _id will be shown if not specify different 
        if not '_id' in fields:
            fields_to_show['_id'] = 0
        results = self.get_collection(collection_name).find_one(query,fields_to_show)
        if results is not None:
            json_object = {key : value for key, value in results.items()}
        return json_object

    def insert_document(self, collection_name, document):
        collection = self.db[collection_name]
        uid = collection.insert_one(document)
        return uid.inserted_id

def rmq_send(self, msg_exchange, msg_routing_key, msg_body):
    self.channel.basic_publish(
        exchange=msg_exchange,
        routing_key=msg_routing_key,
        body=msg_body)

def on_new_client(conn, addr, buffer_size, mdb_handler):
    while True:
        msg = conn.recv(buffer_size)
        if not msg:
            break
        json_msg = json.loads(msg)
        handle_message(json_msg, mdb_handler)
    conn.close()

def handle_message(json_msg: dict, mdb_handler: MongodbHandler):
    if json_msg['Stage'] == 'Progression' and json_msg['MessageType'] == 'Peer_Offer':
        new_device(json_msg=json_msg, mdb_handler=mdb_handler)
    if json_msg['Stage'] == 'Progression' and json_msg['MessageType'] == 'Peer_down':
        device_down(ip=json_msg['ModemAddress'], mdb_handler=mdb_handler)
    device_id = get_id_by_ip(ip=json_msg['ModemAddress'], mdb_handler=mdb_handler)
    json_msg['DeviceId'] = device_id
    insert_msg_to_db(json_msg=json_msg, mdb_handler=mdb_handler)

def new_device(json_msg: dict, mdb_handler: MongodbHandler):
    '''handles new device situation, MongoDBwise and RMQwise'''
    pass

def insert_msg_to_db(json_msg: dict, mdb_handler: MongodbHandler):
    '''inserts the given json message to mongodb'''
    pass

def device_down(ip: str, mdb_handler: MongodbHandler):
    '''handles device down, sends over rmq on device_down queue the device_id'''
    pass

def get_id_by_ip(ip: str, mdb_handler: MongodbHandler):
    '''checks on the db and returns the device_id by the given ip'''
    pass

print('im at the start of rmq-mdb-service')
# tcp server configuration
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
host = '127.0.0.1'
port = 12345
buffer_size = 2048
sock.bind((host, port))  
sock.listen(5)

# rabbitmq configuration (direct queue)
rabbitmq_host = os.getenv('RMQ_HOST')
queue_names = os.getenv('QUEUE_NAMES').split(',')
rmq_connection = pika.BlockingConnection(pika.ConnectionParameters(rabbitmq_host))
channel = rmq_connection.channel()
# declaring the queues
for queue_name in queue_names:
    channel.queue_declare(queue=queue_name)

# mongodb configuration
mdb_handler = MongodbHandler()

while True:
    # Accepting connection from client
    conn, addr = sock.accept()
    _thread.start_new_thread(on_new_client, (conn, addr, buffer_size, mdb_handler))
sock.close()
rmq_connection.close()
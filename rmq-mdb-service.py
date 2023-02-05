# for rabbitmq use
import pika
# for socket use
import socket, os
# thread for using this server as multi-client available
import _thread
# for converting json string into proper json
import json

def on_new_client(conn, addr, buffer_size):
    while True:
        msg = conn.recv(buffer_size)
        if not msg:
            break
        json_msg = json.loads(msg)
        handle_message(json_msg)
    conn.close()

def handle_message(json_msg: dict):
    #channel.basic_publish(exchange='',
    #                  routing_key=queue_name,
    #                  body=data)
    print('aaaaaaaaaaaaaaaaaaaaaa\nbuffer recieved:\n{}\naaaaaaaaaaaaaaaaaa\n'.format(json_msg))

print('im at the start of rmq-mdb-service')
# tcp server configuration
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
host = '127.0.0.1'
port = 12345
buffer_size = 2048
sock.bind((host, port))  
sock.listen(5)

# rabbitmq configuration (direct queue)
queue_name = 'device_ids'
rabbitmq_host = os.getenv('RMQ_HOST')
rmq_connection = pika.BlockingConnection(pika.ConnectionParameters(rabbitmq_host))
channel = rmq_connection.channel()
channel.queue_declare(queue=queue_name)

while True:
    # Accepting connection from client
    conn, addr = sock.accept()
    _thread.start_new_thread(on_new_client, (conn, addr, buffer_size))
sock.close()
rmq_connection.close()
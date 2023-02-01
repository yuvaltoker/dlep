# for rabbitmq use
import pika

# for socket use
import socket, os

import sys

def handle_message(buffer: str):
    print('aaaaaaaaaaaaaaaaaaaaaa\nbuffer recieved:\n{}\naaaaaaaaaaaaaaaaaa\n'.format(buffer))

# tcp server configuration
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  
host = '127.0.0.1'
port = 12345
buffer_size = 1024
sock.bind((host, port))  


# rabbitmq configuration (direct queue)
queue_name = 'device_ids'
rabbitmq_host = os.getenv('RMQ_HOST')
rmq_connection = pika.BlockingConnection(pika.ConnectionParameters(rabbitmq_host))
channel = rmq_connection.channel()
channel.queue_declare(queue=queue_name)

# the life of the daemon overhere
while True:     
    data = sock.recvfrom(buffer_size) # buffer size is 1024 bytes 
    handle_message(data)
    #channel.basic_publish(exchange='',
    #                  routing_key=queue_name,
    #                  body=data)
	
rmq_connection.close()
tcpsocket_connection.close()
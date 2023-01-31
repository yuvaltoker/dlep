# for rabbitmq use
import pika

# for socket use
import socket,os

import sys

def handle_message(buffer: str):
    print('aaaaaaaaaaaaaaaaaaaaaa\nbuffer recieved:\n{}\naaaaaaaaaaaaaaaaaa\n'.format(buffer))

# tcp server configuration
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
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

# i don't know exactly why 5, but here's the explanation from the internet
# listen tells the socket library that we want it to queue up as many as 5 connect requests (the normal max) before refusing outside connections.
sock.listen(5)  
tcpsocket_connection,address = sock.accept()

# the life of the daemon overhere
while True:     
    buff = tcpsocket_connection.recv(buffer_size)  
    print(buff)
    handle_message(buff)
    sys.stdout.flush()
    channel.basic_publish(exchange='',
                      routing_key=queue_name,
                      body=buff)
	
rmq_connection.close()
tcpsocket_connection.close()
# for rabbitmq use
import pika

# for socket use
import socket, os

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

sock.listen()

# Accepting connection from client
conn, addr = sock.accept()

print('3- before running the while loop\n')
# the life of the daemon overhere
while True:
    data = conn.recv(buffer_size) # Raw data from client
    #text = data.decode('utf-8') # Decoding it
    print('at the loop\n')
    handle_message(data)
    #channel.basic_publish(exchange='',
    #                  routing_key=queue_name,
    #                  body=data)
	
rmq_connection.close()
tcpsocket_connection.close()
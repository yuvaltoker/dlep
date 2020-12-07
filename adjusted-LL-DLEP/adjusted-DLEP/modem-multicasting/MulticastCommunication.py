# example of multicasting sender followed by the instruction of -
# https://bip.weizmann.ac.il/course/python/PyMOTW/PyMOTW/docs/socket/multicast.html

import socket
import struct
import sys
import threading
import time
import redis
from threading import Thread, Lock, Event

mutex = Lock()
event = Event()
event.set()

class multicastSenderHandler(threading.Thread):
    def __init__(self, routerIP):
        threading.Thread.__init__(self)
        self.routerIP = routerIP

    def sendMulticast(self, message):
        multicast_group = ('224.3.29.71', 10000)

        # Create the datagram socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        # Set a timeout so the socket does not block indefinitely when trying
        # to receive data.
        sock.settimeout(0.2)

        # Set the time-to-live for messages to 1 so they do not go past the
        # local network segment.
        ttl = struct.pack('b', 1)
        sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl)

        try:

            # Send data to the multicast group
            # print('sending "%s"' % message, file = sys.stderr)
            sock.sendto(message.encode(), multicast_group)

            # Look for responses from all recipients - on the receiver handler thread
            #while True:
            #    print('waiting to receive', file = sys.stderr)
            #    try:
            #        data, server = sock.recvfrom(16)
            #    except socket.timeout:
            #        print('timed out, no more responses', file = sys.stderr)
            #        break
            #    else:
            #        data = data.decode()
            #        print('received "%s" from %s' % (data, server), file = sys.stderr)
                    

        finally:
            # print('closing socket', file = sys.stderr)
            sock.close()

    def run(self):
        while event.is_set():
            self.sendMulticast(self.routerIP)
            time.sleep(2)

class multicastReceiverHandler(threading.Thread):
    def __init__(self, routerIP):
        threading.Thread.__init__(self)
        self.routerIP = routerIP

    def receiveMulticast(self):
        multicast_group = '224.3.29.71'
        server_address = ('', 10000)

        # Create the socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        # Bind to the server address
        sock.bind(server_address)

        # Tell the operating system to add the socket to the multicast group
        # on all interfaces.
        group = socket.inet_aton(multicast_group)
        mreq = struct.pack('4sL', group, socket.INADDR_ANY)
        sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

        # Receive/respond loop
        while event.is_set():
            print('\nwaiting to receive message', file = sys.stderr)
            data, address = sock.recvfrom(1024)
            data = data.decode()
            while data == self.routerIP:
                data, address = sock.recvfrom(1024)
                data = data.decode()
            print('received %s bytes from %s' % (len(data), address), file = sys.stderr)
            print('heartbeat from modem of router %s' % data, file = sys.stderr)

            # print('sending acknowledgement to', address, file = sys.stderr)
            # self.updateRouterOnDB(data) - yet to be made

    def run(self):
        if event.is_set():
            self.receiveMulticast()

def main(routerIP):
    mCastSenderHandler = multicastSenderHandler(str(routerIP))
    mCastSenderHandler.start()
    mcastReceiverHandler = multicastReceiverHandler(str(routerIP))
    mcastReceiverHandler.start()
    


if __name__ == "__main__":
    main(sys.argv[1:])
    
            
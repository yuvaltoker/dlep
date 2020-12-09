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
client = redis.Redis(host = 'redis_db', port = 6379)
routers = []

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
            self.updateRouterOnDB(data)

    def isFamiliarWithRouterIP(self, other_routerIP):
        global routers
        for routerIP in routers:
            if other_routerIP == routerIP:
                return True
        return False

    def updateRouterOnDB(self, other_routerIP):
        global client
        if not self.isFamiliarWithRouterIP(other_routerIP):
            global routers
            routers.append(other_routerIP)
            print('router %s is sconnected' % other_routerIP, file = sys.stderr)
        # sets this router's perspective view of other_routerIP to 0, means just got heartbeat from other_routerIP 
        client.hset(self.routerIP, other_routerIP, str(0))

    def run(self):
        if event.is_set():
            self.receiveMulticast()

class multicastCheckStatusHandler(threading.Thread):
    def __init__(self, routerIP):
        threading.Thread.__init__(self)
        self.routerIP = routerIP

    def checkHeartbeatAndIncrement(self):
        global routers
        global client
        dead_routers = []
        for other_routerIP in routers:
            lastHeartbeat = int(client.hget(self.routerIP, other_routerIP))
            print('lastHeartbeat of router %s is %d' %(other_routerIP, lastHeartbeat), file = sys.stderr)
            if(lastHeartbeat > 3):
                dead_routers.append(other_routerIP)
            else:
                client.hset(self.routerIP, other_routerIP, str(lastHeartbeat + 1))
        
        return dead_routers

    def deleteDeadRouters(self, dead_routers):
        global routers
        for dead in dead_routers:
            routers.remove(dead)
            print('router %s disconnected' % dead, file = sys.stderr)
    
    def run(self):
        time.sleep(2)
        while event.is_set():
            dead_routers = self.checkHeartbeatAndIncrement()
            self.deleteDeadRouters(dead_routers)
            time.sleep(2)    

def main(routerIP):
    print(routerIP, file = sys.stderr)

    mCastSenderHandler = multicastSenderHandler(str(routerIP))
    mCastSenderHandler.start()

    mcastReceiverHandler = multicastReceiverHandler(str(routerIP))
    mcastReceiverHandler.start()

    # yet to be checked
    mcastCheckStatusHandler = multicastCheckStatusHandler(str(routerIP))
    mcastCheckStatusHandler.start()

    


if __name__ == "__main__":
    main(sys.argv[1])
    
            
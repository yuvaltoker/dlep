# example of multicasting sender followed by the instruction of -
# https://bip.weizmann.ac.il/course/python/PyMOTW/PyMOTW/docs/socket/multicast.html

import socket
import struct
import sys
import redis

message = 'very important data'
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
    print('sending "%s"' % message, file = sys.stderr)
    sent = sock.sendto(message.encode(), multicast_group)

    # Look for responses from all recipients
    while True:
        print('waiting to receive', file = sys.stderr)
        try:
            data, server = sock.recvfrom(16)
        except socket.timeout:
            print('timed out, no more responses', file = sys.stderr)
            break
        else:
            data = data.decode()
            print('received "%s" from %s' % (data, server), file = sys.stderr)

finally:
    print('closing socket', file = sys.stderr)
    sock.close()
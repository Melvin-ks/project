import socket
from ecc import *

curve = CurveFp(p, a, b)
G = Point(curve, Gx, Gy)  



HOST = '127.0.0.1'
PORT = 8090
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client.bind((HOST, PORT))

print("connected")


#d2 = randint(1,n-1)
d2=0xd851bc05d872f70cef210965f114e7013489b013a6b841e1a7718b8ee8ebd049

x,addr = client.recvfrom(1024)
y,addr = client.recvfrom(1024)

x = int(x.decode(),16)
y = int(y.decode(),16)

T1=Point(curve,x,y)

T2 = inv_mod(d2,n)*T1
client.sendto(hex(T2.x).encode('utf-8'),addr)
client.sendto(hex(T2.y).encode('utf-8'),addr)



print("decrypt finished")
client.close()
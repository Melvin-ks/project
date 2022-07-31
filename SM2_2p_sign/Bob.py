import socket
from random import randint
from ecc import *



curve = CurveFp(p, a, b)
G = Point(curve, Gx, Gy)  



HOST = '127.0.0.1'
PORT = 8090
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client.bind((HOST, PORT))

print("connected")


d2 = randint(1,n-1)


x,addr = client.recvfrom(1024)

x = int(x.decode(),16)
y,addr = client.recvfrom(1024)
y = int(y.decode(),16)

P1=Point(curve,x,y)

P = inv_mod(d2,p)*P1
P = P+G


x,addr = client.recvfrom(1024)
x = int(x.decode(),16)
y,addr = client.recvfrom(1024)
y = int(y.decode(),16)
Q1 = Point(curve,x,y)
e,addr = client.recvfrom(1024)
e = int(e.decode(),16)

k2 = randint(1,n-1)
k3 = randint(1,n-1)

Q2 = k2*G

Q3=k3*Q1+Q2
x1=Q3.x
r =(x1 + e)%n
s2 = (d2 * k3)%n
s3 = (d2 * (r+k2))%n


client.sendto(hex(r).encode(),addr)
client.sendto(hex(s2).encode(),addr)
client.sendto(hex(s3).encode(),addr)

print("Sign finished")
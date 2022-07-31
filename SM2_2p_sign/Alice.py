from gmssl import sm3,func
import sys
import socket
import binascii
from random import randint
from ecc import *

curve = CurveFp(p, a, b)
G = Point(curve, Gx, Gy)  

HOST = '127.0.0.1'
PORT = 8090
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    client.connect((HOST, PORT))
    print("Client 1 connected!")
except Exception:
    print('Connection failed!')
    sys.exit()
else:

    d1 = randint(1,n-1)
    P1 = inv_mod(d1,p)*G
    
    addr = (HOST, PORT)
   
    client.sendto(str(hex(P1.x)).encode('utf-8'), addr)
    client.sendto(str(hex(P1.y)).encode('utf-8'), addr)
   
    m = "I'm Alice"
    ID_A = "202000460103"
    m = hex(int(binascii.b2a_hex(m.encode()).decode(), 16)).upper()[2:]  #编码为hex
    
    ID_A = hex(int(binascii.b2a_hex(ID_A.encode()).decode(), 16)).upper()[2:]
    ENTL_A = '{:04X}'.format(len(ID_A) * 4)
    ma = ENTL_A + ID_A + '{:064X}'.format(a) + '{:064X}'.format(b) + '{:064X}'.format(Gx) + '{:064X}'.format(Gy)
    ZA = sm3.sm3_hash(func.bytes_to_list(ma.encode()))
    e = sm3.sm3_hash(func.bytes_to_list((ZA + m).encode()))
    
    
    k1 = randint(1,n-1)

    
    Q1 = k1*G
    
    client.sendto(str(hex(Q1.x)).encode('utf-8'),addr)
    client.sendto(str(hex(Q1.y)).encode('utf-8'),addr)
    client.sendto(e.encode('utf-8'),addr)


    r,addr = client.recvfrom(1024)
    r = int(r.decode(),16)
    s2,addr = client.recvfrom(1024)
    s2 = int(s2.decode(),16)
    s3,addr = client.recvfrom(1024)
    s3 = int(s3.decode(),16)


    s=((d1 * k1) * s2 + d1 * s3 - r)%n
    if s!=0 or s!= n - r:
        print("signature:")
        print((hex(r),hex(s)))
    client.close()
from gmssl import sm3,func
import sys
import socket
from random import randint
from ecc import *
from encrypt import encrypt
from kdf import *
curve = CurveFp(p, a, b)
G = Point(curve, Gx, Gy)  

HOST = '127.0.0.1'
PORT = 8090
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    client.connect((HOST, PORT))
    print("connected success!")
except Exception:
    print('Connected failed')
    sys.exit()
else:

    (C1,C2,C3)=encrypt(202000460103)
  
    C1=Point(curve,C1.x,C1.y)
    klen = len(hex(C1.x)[2:])*2
    #d1 = randint(1,n-1)
    d1=0x513ad4e4b26f109f17e5d35f28eb4bef9f962dab7e94762f0e5d94a85faf9e9a
    M=randint(1,n-1)
  
    T1 = C1*inv_mod(d1,n)
    
    
    addr = (HOST, PORT)
    
    client.sendto(str(hex(T1.x)).encode('utf-8'), addr)
    client.sendto(str(hex(T1.y)).encode('utf-8'), addr)


    T2x,addr = client.recvfrom(1024)
    T2y,addr = client.recvfrom(1024)
    T2x=int(T2x.decode(),16)
    T2y=int(T2y.decode(),16)
    T2 = Point(curve, T2x, T2y) 
 

    T3=T2+C1.invert()
  
    x2=T3.x
    y2=T3.y

    t = KDF1(x2 + y2, klen)

    
    M2=C2^int(t,16)
    
    
    
    u = sm3.sm3_hash(func.bytes_to_list(str(x2+M2+y2).encode()))
    

    if(u==C3):
        print("decrypt success!")
        print("  decrypted message: ",M2)
    

    client.close()
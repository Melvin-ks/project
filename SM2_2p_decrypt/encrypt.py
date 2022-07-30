from gmssl import sm3,func
from random import randint
from ecc import *
from kdf import *


curve = CurveFp(p, a, b)
G = Point(curve, Gx, Gy)  

d1=0x513ad4e4b26f109f17e5d35f28eb4bef9f962dab7e94762f0e5d94a85faf9e9a
d2=0xd851bc05d872f70cef210965f114e7013489b013a6b841e1a7718b8ee8ebd049



def encrypt(M):
    print("encrypted message: ",M)
    k=randint(1,n-1)
    C1=k*G
   
    d=inv_mod(d1*d2,n)-1
    P=d*G
    kp=k*P
    x2=kp.x
    y2=kp.y
    
    klen = len(hex(C1.x)[2:])*2
    t = KDF1(x2 + y2, klen)

    
    C2=(M)^int(t,16)
    
    

    C3 = sm3.sm3_hash(func.bytes_to_list(str(x2+M+y2).encode()))
    
    return (C1,C2,C3)






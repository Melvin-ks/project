from gmssl import sm3,func
from math import ceil
from binascii import a2b_hex

class DataPrimitives():
        def __init__(self, explain=False):
            self.explain = explain
    
        def I2OSP(self, longint, length):
        
            hex_string = '%X' % longint
            if len( hex_string ) > 2 * length:
                    raise ValueError( 'integer %i too large to encode in %i octets' % ( longint, length ) )
            return a2b_hex(  hex_string.zfill( 2 * length ) )

cp = DataPrimitives(0)


def KDF1(x,l):
    assert l >= 0, 'l should be positive integer' 
    
    k = l / float(32)
    k = int(ceil(k))
    l_str = ''
    for i in range(0,k):        
        l_str = l_str+sm3.sm3_hash(func.bytes_to_list(str(x).encode()+cp.I2OSP(i,4)))
    
    return l_str[:l*2]


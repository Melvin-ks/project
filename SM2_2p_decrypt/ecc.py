import random


p  = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
a  = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC
b  = 0x28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93
n  = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123
Gx = 0x32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7
Gy = 0xBC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0




def inv_mod(b, p):
    if b < 0 or p <= b:
        b = b % p
    c, d = b, p
    uc, vc, ud, vd, temp = 1, 0, 0, 1, 0
    while c != 0:
        temp = c
        q, c, d = d // c, d % c, temp
        uc, vc, ud, vd = ud - q * uc, vd - q * vc, uc, vc

    assert d == 1
    if ud > 0:
        return ud
    else:
        return ud + p


def leftmost_bit(x):
    assert x > 0
    result = 1
    while result <= x:
        result = 2 * result
    return result // 2


class CurveFp(object):

    def __init__(self, p, a, b):
       
        self.p = p
        self.a = a
        self.b = b

    def contains_point(self, x, y):
        return (y * y - (x * x * x + self.a * x + self.b)) % self.p == 0

    def show_all_points(self):
        return [(x, y) for x in range(self.p) for y in range(self.p) if
                (y * y - (x * x * x + self.a * x + self.b)) % self.p == 0]

    def __repr__(self):
        return "Curve(p={0:d}, a={1:d}, b={2:d})".format(self.p, self.a, self.b)




class Point(object):

    def __init__(self, curve, x, y, order=None):

        self.curve = curve
        self.x = x
        self.y = y
        self.order = order
        
        if self.curve:
            assert self.curve.contains_point(x, y)
        if order:
            assert self * order == INFINITY

    def __eq__(self, other):
        
        if self.curve == other.curve \
                and self.x == other.x \
                and self.y == other.y:
            return True
        else:
            return False

    def __add__(self, other):
        """两个点‘相加’"""

        if other == INFINITY:
            return self
        if self == INFINITY:
            return other
        assert self.curve == other.curve

        if self.x == other.x:
            if (self.y + other.y) % self.curve.p == 0:
                return INFINITY
            else:
                return self.double()

        p = self.curve.p
        l = ((other.y - self.y) * \
             inv_mod(other.x - self.x, p)) % p

        x3 = (l * l - self.x - other.x) % p
        y3 = (l * (self.x - x3) - self.y) % p

        return Point(self.curve, x3, y3)

    def __mul__(self, other):
        e = other
        if self.order:
            e = e % self.order
        if e == 0:
            return INFINITY
        if self == INFINITY:
            return INFINITY

        e3 = 3 * e
        negative_self = Point(self.curve, self.x, -self.y, self.order)
        i = leftmost_bit(e3) // 2
        result = self

        while i > 1:
            result = result.double()
            if (e3 & i) != 0 and (e & i) == 0:
                result = result + self
            if (e3 & i) == 0 and (e & i) != 0:
                result = result + negative_self
            i = i // 2
        return result

    def __rmul__(self, other):
        """一个点乘以一个整数"""
        return self * other

    def __repr__(self):
        if self == INFINITY:
            return "infinity"
        return "({0},{1})".format(self.x, self.y)

    def double(self):
        """the double point."""
        if self == INFINITY:
            return INFINITY

        p = self.curve.p
        a = self.curve.a
        l = ((3 * self.x * self.x + a) * \
             inv_mod(2 * self.y, p)) % p

        x3 = (l * l - 2 * self.x) % p
        y3 = (l * (self.x - x3) - self.y) % p

        return Point(self.curve, x3, y3)

    def invert(self):
        if self.y is None:
            return Point(None, None, None)
        return Point(self.curve, self.x, -self.y % self.curve.p)




INFINITY = Point(None, None, None)



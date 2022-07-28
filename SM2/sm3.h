#include <string.h>

#define SM3_len 256
#define SM3_T0 0x79CC4519
#define SM3_T1 0xF3988A32
#define SM3_T2 0xE7311465
#define SM3_T3 0xCE6228CB
#define SM3_T4 0x9CC45197
#define SM3_T5 0x3988A32F
#define SM3_T6 0x7311465E
#define SM3_T7 0xE6228CBC
#define SM3_T8 0xCC451979
#define SM3_T9 0x988A32F3
#define SM3_T10 0x311465E7
#define SM3_T11 0x6228CBCE
#define SM3_T12 0xC451979C
#define SM3_T13 0x88A32F39
#define SM3_T14 0x11465E73
#define SM3_T15 0x228CBCE6
#define SM3_T16 0x9D8A7A87
#define SM3_T17 0x3B14F50F
#define SM3_T18 0x7629EA1E
#define SM3_T19 0xEC53D43C
#define SM3_T20 0xD8A7A879
#define SM3_T21 0xB14F50F3
#define SM3_T22 0x629EA1E7
#define SM3_T23 0xC53D43CE
#define SM3_T24 0x8A7A879D
#define SM3_T25 0x14F50F3B
#define SM3_T26 0x29EA1E76
#define SM3_T27 0x53D43CEC
#define SM3_T28 0xA7A879D8
#define SM3_T29 0x4F50F3B1
#define SM3_T30 0x9EA1E762
#define SM3_T31 0x3D43CEC5
#define SM3_T32 0x7A879D8A
#define SM3_T33 0xF50F3B14
#define SM3_T34 0xEA1E7629
#define SM3_T35 0xD43CEC53
#define SM3_T36 0xA879D8A7
#define SM3_T37 0x50F3B14F
#define SM3_T38 0xA1E7629E
#define SM3_T39 0x43CEC53D
#define SM3_T40 0x879D8A7A
#define SM3_T41 0xF3B14F5
#define SM3_T42 0x1E7629EA
#define SM3_T43 0x3CEC53D4
#define SM3_T44 0x79D8A7A8
#define SM3_T45 0xF3B14F50
#define SM3_T46 0xE7629EA1
#define SM3_T47 0xCEC53D43
#define SM3_T48 0x9D8A7A87
#define SM3_T49 0x3B14F50F
#define SM3_T50 0x7629EA1E
#define SM3_T51 0xEC53D43C
#define SM3_T52 0xD8A7A879
#define SM3_T53 0xB14F50F3
#define SM3_T54 0x629EA1E7
#define SM3_T55 0xC53D43CE
#define SM3_T56 0x8A7A879D
#define SM3_T57 0x14F50F3B
#define SM3_T58 0x29EA1E76
#define SM3_T59 0x53D43CEC
#define SM3_T60 0xA7A879D8
#define SM3_T61 0x4F50F3B1
#define SM3_T62 0x9EA1E762
#define SM3_T63 0x3D43CEC5


#define SM3_p1(x) (x^SM3_rotl32(x,15)^SM3_rotl32(x,23))
#define SM3_p0(x) (x^SM3_rotl32(x,9)^SM3_rotl32(x,17))
#define SM3_ff0(x,y,z) (x^y^z)
#define SM3_ff1(x,y,z) ((x&y)|(x&z)|(y&z))
#define SM3_gg0(x,y,z) (x^y^z)
#define SM3_gg1(x,y,z) ((x&y)|((~x)&z))
#define SM3_rotl32(x,n) ((((uint32_t) x) << n) | (((uint32_t) x) >> (32 - n)))
#define SM3_rotr32(x,n) ((((uint32_t) x) >> n) | (((uint32_t) x) << (32 - n)))


static uint32_t SM3_T[64] = {SM3_T0,SM3_T1,SM3_T2,SM3_T3,SM3_T4,SM3_T5,SM3_T6,SM3_T7,
                             SM3_T8,SM3_T9,SM3_T10,SM3_T11,SM3_T12,SM3_T13,SM3_T14,SM3_T15,
                             SM3_T16,SM3_T17,SM3_T18,SM3_T19,SM3_T20,SM3_T21,SM3_T22,SM3_T23,
                             SM3_T24,SM3_T25,SM3_T26,SM3_T27,SM3_T28,SM3_T29,SM3_T30,SM3_T31,
                             SM3_T32,SM3_T33,SM3_T34,SM3_T35,SM3_T36,SM3_T37,SM3_T38,SM3_T39,
                             SM3_T40,SM3_T41,SM3_T42,SM3_T43,SM3_T44,SM3_T45,SM3_T46,SM3_T47,
                             SM3_T48,SM3_T49,SM3_T50,SM3_T51,SM3_T52,SM3_T53,SM3_T54,SM3_T55,
                             SM3_T56,SM3_T57,SM3_T58,SM3_T59,SM3_T60,SM3_T61,SM3_T62,SM3_T63};
                                 

typedef struct {
	uint32_t state[8];
	uint32_t length;
	uint32_t curlen;
	uint8_t buf[64];
} sm3_ctx;





void extension_W(uint32_t Bi[], uint32_t Wj[]);
void extension_W1(uint32_t Wj[], uint32_t Wj1[]);
void CF(uint32_t Wj[], uint32_t Wj1[], uint32_t V[]);
void BigEndian(uint8_t src[], uint32_t bytelen, uint8_t des[]);
void SM3_init(sm3_ctx *ctx);
void SM3_compress(sm3_ctx * ctx);
void SM3_process(sm3_ctx * ctx, uint8_t buf[], int len);
void SM3_final(sm3_ctx *ctx, uint8_t *hash);
void SM3_256(uint8_t buf[], int len, uint8_t hash[]);



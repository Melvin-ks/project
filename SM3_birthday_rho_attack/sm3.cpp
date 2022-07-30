#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <iostream>
#include "sm3.h"
#include<cmath>




void extension_W(uint32_t Bi[], uint32_t W[])
{
	int i;
	uint32_t tmp;
	for(i=0;i<=15;i++)
	{
		W[i]=Bi[i];
	}
	for(i=16;i<=67;i++)
	{
		tmp=W[i-16]
			^ W[i-9]
			^ SM3_rotl32(W[i-3],15);
		W[i]=SM3_p1(tmp)
			^ (SM3_rotl32(W[i-13],7))
			^ W[i-6];
	}
}

void extension_W1(uint32_t W[], uint32_t W1[])
{
	int i;
	for(i=0;i<=63;i++)
	{
		W1[i]=W[i]^W[i+4];
	}
}

void CF(uint32_t W[], uint32_t W1[], uint32_t V[])
{
	uint32_t SS1;
	uint32_t SS2;
	uint32_t TT1;
	uint32_t TT2;
	uint32_t A,B,C,D,E,F,G,H;
	uint32_t FF;
	uint32_t GG;
	int j;
	
	A=V[0];
	B=V[1];
	C=V[2];
	D=V[3];
	E=V[4];
	F=V[5];
	G=V[6];
	H=V[7];
	for(j=0;j<=63;j++)
	{
		SS1=SM3_rotl32((SM3_rotl32(A,12)+E+SM3_T[j]),7);
		
		SS2=SS1^SM3_rotl32(A,12);
		
		if(j<=15)
		{
			FF=SM3_ff0(A,B,C);
		}
		else
		{
			FF=SM3_ff1(A,B,C);
		}
		TT1=FF+D+SS2+*W1;
		W1++;
		
		if(j<=15)
		{
			GG=SM3_gg0(E,F,G);
		}
		else
		{
			GG=SM3_gg1(E,F,G);
		}
		TT2=GG+H+SS1+*W;
		W++;
		
		D=C;
		
		C=SM3_rotl32(B,9);
		
		B=A;
		
		A=TT1;
		
		H=G;
		G=SM3_rotl32(F,19);
		
		F=E;
		
		E=SM3_p0(TT2);
		
	}
	//update V
	V[0]=A^V[0];
	V[1]=B^V[1];
	V[2]=C^V[2];
	V[3]=D^V[3];
	V[4]=E^V[4];
	V[5]=F^V[5];
	V[6]=G^V[6];
	V[7]=H^V[7];
	
}

void BigEndian(uint8_t src[], uint32_t bytelen, uint8_t des[])
{
	uint8_t tmp = 0;
	uint32_t i = 0;
	for(i=0; i<bytelen/4; i++)
	{
		tmp = des[4*i];des[4*i] = src[4*i+3];
		src[4*i+3] = tmp;
		tmp = des[4*i+1];
		des[4*i+1] = src[4*i+2];
		des[4*i+2] = tmp;
	}
}

void SM3_init(sm3_ctx *ctx)
{
	ctx->curlen = ctx->length = 0;
	ctx->state[0] = 0x7380166f;
	ctx->state[1] = 0x4914b2b9;
	ctx->state[2] = 0x172442d7;
	ctx->state[3] = 0xda8a0600;
	ctx->state[4] = 0xa96f30bc;
	ctx->state[5] = 0x163138aa;
	ctx->state[6] = 0xe38dee4d;
	ctx->state[7] = 0xb0fb0e4e;
}

void SM3_compress(sm3_ctx * ctx)
{
	uint32_t W[68];
	uint32_t W1[64];
	BigEndian(ctx->buf, 64, ctx->buf);
	extension_W((uint32_t *)ctx->buf,W);
	extension_W1(W,W1);
	CF(W, W1, ctx->state);
}

void SM3_process(sm3_ctx * ctx, uint8_t *buf, int len)
{
	while (len--)
	{
		
		ctx->buf[ctx->curlen] = *buf++;
		ctx->curlen++;
		
		if (ctx->curlen == 64)
		{
			SM3_compress(ctx);
			ctx->length += 512;
			ctx->curlen = 0;
		}}
}

void SM3_final(sm3_ctx *ctx, uint8_t hash[])
{
	int i;
	uint8_t tmp = 0;
	
	ctx->length += ctx->curlen <<3;
	
	ctx->buf[ctx->curlen] = 0x80;
	ctx->curlen++;
	
	if (ctx->curlen >56)
	{
		for (; ctx->curlen < 64;)
		{
			ctx->buf[ctx->curlen] = 0;
			ctx->curlen++;
		}
		SM3_compress(ctx);
		ctx->curlen = 0;
	}
	
	for (; ctx->curlen < 56;)
	{ctx->buf[ctx->curlen] = 0;
		ctx->curlen++;
	}
	
	for (i = 56; i < 60; i++)
	{
		ctx->buf[i] = 0;
	}
	
	ctx->buf[63] = ctx->length & 0xff;
	ctx->buf[62] = (ctx->length >> 8) & 0xff;
	ctx->buf[61] = (ctx->length >> 16) & 0xff;
	ctx->buf[60] = (ctx->length >> 24) & 0xff;
	SM3_compress(ctx);
	
	memcpy(hash,ctx->state,SM3_len/8);
	BigEndian(hash,SM3_len/8,hash);
}

void SM3_256(uint8_t buf[], int len, uint8_t hash[])
{
	sm3_ctx ctx;
	SM3_init(&ctx);
	SM3_process(&ctx, buf, len);
	SM3_final(&ctx, hash);
}

void sm3_hash_rho(uint8_t buf[], int len, uint8_t hash[])
{
	unsigned char hash_value[32];
	sm3_ctx ctx;
	SM3_init(&ctx);
	SM3_process(&ctx, buf, len);
	SM3_final(&ctx, hash_value);
	memcpy(hash,hash_value,bit);
}



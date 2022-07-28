#include <stdio.h>
#include <iostream>
#include <string.h>
#include "sm3.h"
#include <random>
#include <ctime>
#include <chrono>
using namespace std;

bool equal(unsigned char* hash1,unsigned char* hash2)
{
    for(int i=0;i<bit;i++)
        if(hash1[i]!=hash2[i])
            return false;
    return true;
        
}




void random_input(unsigned char input[bit])
{
    static default_random_engine e;
    e.seed(time(0));
    for (int i = 0; i <bit ; i++)
    {
            input[i]= e();
    }
}


int searchj0(unsigned char* h0,unsigned char* h)
{
   
    unsigned int hash_length;
    unsigned char h_prime[bit],buffer[bit];
    unsigned char hash1[32],hash2[32];
    sm3_hash_rho(h0, bit, h, &hash_length);
    sm3_hash_rho(h0, bit, buffer, &hash_length);
    sm3_hash_rho(buffer, bit, h_prime, &hash_length);
    int j=1;
   
    vector<unsigned char*> old_h,old_h_prime;
    old_h.emplace_back(h);
    old_h.emplace_back(h);
    old_h_prime.emplace_back(h_prime);
    old_h_prime.emplace_back(h_prime);
    
    //while(strcmp((char*)h,(char*)h_prime))   
    while(!equal(h,h_prime))
    {
        
        sm3_hash_rho(h, bit, h, &hash_length);
        sm3_hash_rho(h_prime, bit, buffer, &hash_length);
        sm3_hash_rho(buffer, bit, h_prime, &hash_length);
        j++;
        //old_h[0]=old_h[1];
        memcpy(old_h[0],old_h[1],bit);
        memcpy(old_h[1],h,bit);

        //old_h_prime[0]=old_h_prime[1];
        memcpy(old_h_prime[0],old_h_prime[1],bit);
        memcpy(old_h_prime[1],h_prime,bit);
    }
    return j;
}


int searchi0(unsigned char* h0,unsigned char* h0_pprime,unsigned char* old_h,unsigned char* old_h_pprime)
{
    unsigned int hash_length;
    unsigned char h[bit],buffer[bit],h_pprime[bit];
    sm3_hash_rho(h0, bit, h, &hash_length);
    sm3_hash_rho(h0_pprime, bit, h_pprime, &hash_length);
    //old_h=h0;   
    memcpy(old_h,h0,bit);
    
    //old_h_pprime=h0;
    memcpy(old_h_pprime,h0,bit);
    int i=1;
   
    //while(strcmp((char*)h,(char*)h_pprime))
    while(!equal(h,h_pprime))
    {
        //old_h=h;
        memcpy(old_h,h,bit);
        //old_h_pprime=h_pprime;
        memcpy(old_h_pprime,h_pprime,bit);
        sm3_hash_rho(h, bit, h, &hash_length);
        sm3_hash_rho(h_pprime, bit, h_pprime, &hash_length);
        i++;
        //printf("i: %d\n",i);
    }
    //printf("i: %d\n",i);
    return i;
}


void collision()
{
    int i0=0;
    int j0=0;
    unsigned int hash_length;
    unsigned char h0[bit],h_pprime[bit],old_h_i0[bit],old_h_pprimei0[bit],plain1[bit],plain2[bit];
    unsigned char hash1[32],hash2[32];
    while(!i0)
    {
        random_input(h0);
        
        j0=searchj0(h0,h_pprime);
        
        i0=searchi0(h0,h_pprime,old_h_i0,old_h_pprimei0);
        
    }
    //hash1=old_h_i0;
    memcpy(plain1,old_h_i0,bit);
    //hash2=old_h_pprimei0;
    memcpy(plain2,old_h_pprimei0,bit);
    sm3_hash(plain1, bit, hash1, &hash_length);
    sm3_hash(plain2, bit, hash2, &hash_length);
    printf("plaintext: \n");
    for(int i=0;i<bit;i++)
        printf("%02x",plain1[i]);
    printf("\n");
    for(int i=0;i<bit;i++)
        printf("%02x",plain2[i]);
    printf("\n");
    printf("hash value: \n");
    for(int i=0;i<32;i++)
        printf("%02x",hash1[i]);
    printf("\n");
    for(int i=0;i<32;i++)
        printf("%02x",hash2[i]);
    printf("\n");
}


int main()
{
    auto start = std::chrono::steady_clock::now();
    collision();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> cost_time = end - start;
    std::cout << "time: " << cost_time.count() << "\n";
    
    

   
    return 0;
}
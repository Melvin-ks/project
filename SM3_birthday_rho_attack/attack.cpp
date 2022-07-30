#include <stdio.h>
#include <iostream>
#include <string.h>
#include "sm3.h"
#include <random>
#include <ctime>
#include <chrono>
using namespace std;

const int N = 1e6;
unsigned char plain[N][65];
unsigned char hash_value[N][33];


bool compare(unsigned char hash1[], unsigned char hash2[], int n)
{

    for (int i = 0; i < n; i++)
        if (hash1[i] > hash2[i])
            return true;
        else if (hash1[i] < hash2[i])
            return false;
    return true;
}



void rand_input(unsigned char input[][65], unsigned char hash_value[][33])
{
    
    unsigned int hash_length;
    static default_random_engine e;
    e.seed(time(0));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < 64; j++)
            input[i][j] = e();
        SM3_256(input[i], 64, hash_value[i]);
        // memcpy(buffer, hash_value[i], sizeof(buffer));//4,5,6,7

        // if (u_set.find(buffer) != u_set.end())
        // {
        //         printf("id:%d\n",i);
        //     }
        // u_set.insert(buffer);
        // printf("size; %d\n",u_set.size());
        // for(int j=0;j<sizeof(buffer)/sizeof(unsigned char);j++)
        //     printf("%02x",buffer[j]);
        // printf("\n");

    }
}

bool cmphash(unsigned char *H1, unsigned char *H2, int len)
{
    for (int i = 0; i < len; i++)
        if (H1[i] != H2[i])
            return false;
    return true;
}

void birday_attack(unsigned char input[][65], unsigned char hash_value[][33], int bit)
{
    int n = bit;
    int i, j, k;
    unsigned char temp[33];
    for (i = 0; i < N - 1; i++)
    {
        for (j = i + 1; j < N; j++)
        {
            bool flag = true;
            for (int k = 0; k < n; k++)
            {
                if (hash_value[i][k] != hash_value[j][k])
                    flag = false;
            }
            if (flag)
            // if(cmphash(hash_value[i],hash_value[j],n))
            {
                printf("plaintext: \n");
                for (int b = 0; b < 64; b++)
                    printf("%02x", input[i][b]);
                printf("\n");
                for (int b = 0; b < 64; b++)
                    printf("%02x", input[j][b]);
                printf("\n");
                printf("hash_value: \n");
                for (int b = 0; b < 32; b++)
                    printf("%02x", hash_value[i][b]);
                printf("\n");
                for (int b = 0; b < 32; b++)
                    printf("%02x", hash_value[j][b]);
                printf("\n");

                // printf("%d  %d\n",i,j);
                return;
            }
        }
    }
}

bool equal(unsigned char* hash1,unsigned char* hash2,int n)
{
    for(int i=0;i<n;i++)
        if(hash1[i]!=hash2[i])
            return false;
    return true;
        
}




int main()
{

    auto start = std::chrono::steady_clock::now();
    
    rand_input(plain, hash_value); //初始化
    printf("initilized\n");
    birday_attack(plain,hash_value,3);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> cost_time = end - start;
    std::cout << "time: " << cost_time.count() << "\n";

    
    return 0;
}

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "sm3.h"
#include <random>
#include <ctime>
#include <unordered_set>
#include <chrono>
using namespace std;

const int N = 1e6;
unsigned char plain[N][65];
unsigned char hash_value[N][33];
unordered_set<unsigned char *> u_set;

bool compare(unsigned char hash1[], unsigned char hash2[], int n)
{

    for (int i = 0; i < n; i++)
        if (hash1[i] > hash2[i])
            return true;
        else if (hash1[i] < hash2[i])
            return false;
    printf("here");
    return true;
}

void quick_sort(unsigned char q[][3], int l, int r, int n)
{
    printf("here");
    if (l >= r)
        return;
    printf("here");
    int i = l - 1, j = r + 1;
    unsigned char *x;
    // copy(begin(q[l + r >> 1]),end(q[l + r >> 1),begin(x));
    printf("here");
    memcpy(x, q[l + r >> 1], sizeof(q[l + r >> 1]));
    while (i < j)
    {
        do
            i++;
        while (compare(x, q[i], n));
        do
            j--;
        while (compare(q[j], x, n));
        if (i < j)
            swap(q[i], q[j]);
    }
    printf("here");
    quick_sort(q, l, j, n), quick_sort(q, j + 1, r, n);
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
        sm3_hash(input[i], 64, hash_value[i], &hash_length);
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
    int n = bit / 8;
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

void CharToInt(unsigned char *dest,unsigned char *src, long len)
{
        int i;
        int value_temp = 0;

        for(i=0;i<len;i++)
        {
                value_temp = value_temp*10 + (*(src+i)-'0');
        }
        int a;
        
        string temp=to_string(value_temp*value_temp+1);
        
        dest=(unsigned char*)temp.c_str();
        printf("%s\n",dest);
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
    birday_attack(plain,hash_value,24);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> cost_time = end - start;
    std::cout << "time: " << cost_time.count() << "\n";

    // unsigned char a[] = "13";
    // unsigned char b[] = {0x10, 0x18, 0x57};
    // // printf("%d", compare(a, b, 3));
    // // unsigned char t[2][3] = {{0x10, 0x18, 0x97}, {0x10, 0x18, 0x57}};

    // // quick_sort(t, 0, N - 1, 3);
    // unsigned char* c;
    // CharToInt(c,a,strlen((char*)a));
    // printf("%s\n",a);
    //Pollard_rho(2);
  
    return 0;
}


#include"SM2_sign.h"
#include<chrono>


using namespace std;

void print_buffer(unsigned char* buffer)
{
	for(int i=0;i<32;i++)
		printf("%02x",buffer[i]);
	printf("\n");
}



int main(void)
{
	unsigned char pubx_char[32], puby_char[32],ZA[32];
	unsigned char r[32], s[32];
	unsigned char* message = (unsigned char*)"sdu";
	big d,pubx,puby;  
	epoint* pub;
	

	auto start = chrono::steady_clock::now();
    
	
	SM2_init();
	SM2_creat_key(&d, &pub);
	
		
	pubx = mirvar(0);
	puby = mirvar(0);
	epoint_get(pub, pubx, puby);
	big_to_bytes(32, pubx, (char*)pubx_char, TRUE);
	big_to_bytes(32, puby, (char*)puby_char, TRUE);
	printf("Generate PubKey:\n\n");
	print_buffer(pubx_char);
	print_buffer(puby_char);
	printf("\n");

	SM2_ZA(pubx_char, puby_char, ZA);

	SM2_sign(message,strlen((char*)message),ZA,d,r,s);

	printf("Generate Signature:\n\n");
	print_buffer(r);
	print_buffer(s);
	printf("\n");
	
	SM2_verify(message, strlen((char*)message), ZA, pubx_char, puby_char, r, s);
	


	auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> cost_time = end - start;
    std::cout << "time: " << cost_time.count() << "\n";
	
	
	

	return 0;
}
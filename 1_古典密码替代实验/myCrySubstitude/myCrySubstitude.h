
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define CRYPT_ERROR 1
#define CRYPT_OK 0

#define DECRYPT_ERROR 1
#define DECRYPT_OK 0

int ToLowerCase(unsigned char* str);
int Decrypt(unsigned char* str,unsigned char* code);
int Encrypt(unsigned char* str,unsigned char* code);
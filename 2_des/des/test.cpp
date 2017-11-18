#include"des.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define PACKETLEN 8
#define RAND_MAX 0x7fff

int main()

{

unsigned char pt[PACKETLEN+1];
unsigned char ct[PACKETLEN+1];
unsigned char key[8]={'a','b','c','d','a','b','c','d'};


	int i = 0;
	int in_len = PACKETLEN;
	time_t t;                 //这两行保证每次产生的随机数不同
    srand( (unsigned) time( &t ) ); 

	pt[in_len] = '\0';

	for(i=0;i<in_len;i++)
		pt[i] = 97+rand()*26/RAND_MAX;

	for(i=0;i<in_len;i++)
		key[i] = 97+rand()*26/RAND_MAX;

des_key skey;

pt[in_len]=ct[in_len]='\0';
des_setup((unsigned char*)key,PACKETLEN,0,&skey);

des_ecb_encrypt(pt,ct,&skey);

des_ecb_decrypt(ct,pt,&skey);

printf("DES明文  %s\n",pt);

printf("DES密文  %s\n",ct);

system("PAUSE");

return 0;

}


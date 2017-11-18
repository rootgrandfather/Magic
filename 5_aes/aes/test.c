#include "RIJNDAEL.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAXNUM 255
#define RAND_MAX 0x7fff

void main()

{
	int i;
	u4byte out_block[4],de_block[4];
	u4byte in_key[4]={0x11111111,0x22222222,0x33333333,0x44444444};
	u4byte plaint_block[4]={0x12121212,0x34343434,0x56565656,0x78787878};
	
	u4byte x1,x2,x3,x4;
 	time_t t;                 //这两行保证每次产生的随机数不同
    srand( (unsigned) time( &t ) ); 

	printf("Start~~~~~~~~~~~~~~\n");
	for(i=0;i<4;i++)
	{
		x1 = rand()*MAXNUM/RAND_MAX;
		x2 = rand()*MAXNUM/RAND_MAX;
		x3 = rand()*MAXNUM/RAND_MAX;
		x4 = rand()*MAXNUM/RAND_MAX;
		in_key[i] = 0;
		in_key[i] = x1;
		in_key[i]<<=8;
		in_key[i] += x2;
		in_key[i]<<=8;
		in_key[i] += x3;
		in_key[i]<<=8;
		in_key[i] += x4;
	}

	for(i=0;i<4;i++)
	{
		x1 = rand()*MAXNUM/RAND_MAX;
		x2 = rand()*MAXNUM/RAND_MAX;
		x3 = rand()*MAXNUM/RAND_MAX;
		x4 = rand()*MAXNUM/RAND_MAX;
		plaint_block[i] = 0;
		plaint_block[i] = x1;
		plaint_block[i]<<=8;
		plaint_block[i] += x2;
		plaint_block[i]<<=8;
		plaint_block[i] += x3;
		plaint_block[i]<<=8;
		plaint_block[i] += x4;
	}

	set_key(in_key,128);

	encrypt(plaint_block,out_block);
	
	printf("Encryption:\n密文： \n");
	for(i=0;i<4;i++)
		printf("%x",out_block[i]);
	printf("\n");

	decrypt(out_block,de_block);
	printf("Decryption!\n恢复原文： \n");
	for(i=0;i<4;i++)
		printf("%x",de_block[i]);
	printf("\n");
}
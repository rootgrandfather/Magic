
#include "idea.c"


#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAXLEN 500
#define RAND_MAX 0x7fff

void main(void)
{
 unsigned char  revalue1[500], revalue2[500];
 //revalue1 is the ciphertext, revalue2 is the deplaintext
 unsigned char key[]="123456789abcdefg";
 //Key is the session key, you can select any 8 bytes as the key
 unsigned char in[]="I am a student. Now i am studing in Haerbin!!!!";
 //in is the plaintext
int i;
 unsigned long outlength;
	int in_len = 0;
 	time_t t;                 //这两行保证每次产生的随机数不同
    srand( (unsigned) time( &t ) ); 

	in_len = rand()*MAXLEN/RAND_MAX;

	for(i=0;i<in_len;i++)
		in[i] = 97+rand()*26/RAND_MAX;

	in[in_len] = '\0';

 printf("Now start!!!!\n");
enideaString(key,in,strlen((char *)in),revalue1,&outlength);
printf("密文: %s\n",revalue1);

deideaString(key,revalue1,outlength,revalue2,&outlength);
printf("恢复原文: %s\n",revalue2);
}
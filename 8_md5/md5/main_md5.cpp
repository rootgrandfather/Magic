

#include "md5.c"


#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define RAND_MAX 0x7fff

void main(void)
{
 unsigned char *inputData;
 char *inp = (char *)malloc(sizeof(char)*1024);
 inputData=(unsigned char*)inp;
 unsigned char* hashstr;

	int i,x;
 	time_t t;                 //这两行保证每次产生的随机数不同
	int len;

    srand( (unsigned) time( &t ) );
	len = rand()*200/RAND_MAX;

	inp[len]='\0';
	for(i=0;i<len;i++)
	{
		x = rand()*16/RAND_MAX;
		if(x<10)
			inp[i] = 48 + x;
		else if(x>9 && x<16)
			inp[i] = 65 + x -10;
	}

	printf("original string is: %s\n",inp);

 // allocate memory for string 
 if ((hashstr = (unsigned char *) malloc(17)) == NULL)
    {
       printf("Not enough memory to allocate buffer\n");
       exit(1);  // terminate program if out of memory 
    }


 MD5String(inputData,hashstr);
 printf("The hash result is %s\n",hashstr);

 }
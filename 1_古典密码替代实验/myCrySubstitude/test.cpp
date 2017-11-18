
#include "myCrySubstitude.h"

#define MAXLEN 1024
#define RAND_MAX 0x7fff

void main()

{

	unsigned char * str;
	unsigned char* code;
	unsigned char* encode;

	int i = 0;
	int in_len = 0;

	str = new unsigned char(MAXLEN+1);
	code = new unsigned char(2);
	encode = new unsigned char(MAXLEN+1);
	time_t t;                 //这两行保证每次产生的随机数不同
    srand( (unsigned) time( &t ) ); 
	
	in_len = rand()*MAXLEN/RAND_MAX;

	str[in_len] = '\0';

       

	for(i=0;i<in_len;i++)
		str[i] = 97+rand()*26/RAND_MAX;

	code[0]=97+rand()*10/RAND_MAX;



	code[1]='\0';

	Encrypt(str,code);
	printf("密文%s\n",str);

	Decrypt(str,code);
	printf("明文%s\n",str);
}
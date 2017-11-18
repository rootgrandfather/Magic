/*
 *   Test program for
 *   Elliptic Curve ElGamal Encryption
 *
 *   It is assumed that Curve parameters are defined in file param.h
 *
 *   The curve is y^2=x^3+Ax+b mod p
 * 
 *	 The file is main.c
 *
 *   Ma ChunGuang 2004-03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "miracl.h"
#include "param.h"
#include <time.h>
#define RAND_MAX 0x7fff
int main()
{
//	FILE *fp;
//  char ifname[50],ch;
	int i;
	int res;
	char *myPriKey, *myPubKey;
	MR_BYTE *message, *plain; 
//	MR_DWORD MSGLen;
	char *mySignature, *myRand_data, *myHidden_data;

	int x;
 	time_t t;                 //这两行保证每次产生的随机数不同
	int len;

/*	initial */
	message=(MR_BYTE *)malloc(sizeof(MR_BYTE)*1024); /* the max length of message is 1024-char */
	for(i=0;i<1024;i++)
		message[i]='\0';
	myPriKey=(char *)malloc(sizeof(char)*NUMBITS);
	myPubKey=(char *)malloc(sizeof(char)*NUMBITS*2);
	mySignature=(char *)malloc(sizeof(char)*NUMBITS*2);
	myHidden_data=(char *)malloc(sizeof(char)*NUMBITS);
	myRand_data=(char *)malloc(sizeof(char)*NUMBITS);
	plain=(MR_BYTE *)malloc(sizeof(MR_BYTE)*1024);

/* Read message from file ifname */
  /*  printf("file to be signed = ");
    gets(ifname);
    if ((fp=fopen(ifname,"rb"))==NULL)
    {
        printf("Unable to open file %s\n",ifname);
		return 0;
    }
	i=0;
	while((ch=fgetc(fp))!=EOF)
	{
		message[i]=ch;
		i++;
	}
	message[i]='\0';
	MSGLen=i;
	fclose(fp);*/

/* Generate PrivateKey and PublicKey for ECC */
	printf("\nGenrate Keys:\n");
	res = ECC_GenKey(myPriKey,myPubKey);
	printf(" The private key: %s\n", myPriKey);
	printf(" The public key: %s\n", myPubKey);



/* Encryptiong and decryption */
//	printf("Pleasse input message(Less than 1024 bytes).\n");
//	scanf("%s",message);


    srand( (unsigned) time( &t ) );
	len = rand()*200/RAND_MAX;

	message[len]='\0';
	for(i=0;i<len;i++)
	{
		x = rand()*16/RAND_MAX;
		if(x<10)
			message[i] = 48 + x;
		else if(x>9 && x<16)
			message[i] = 65 + x -10;
	}

//	strcpy(message,"FFCC1122");
	//strcpy(message,"FFFFFCCCCFFFFFFFFFFFFFFFFFFFFFF1111111111111111BBBB");
	//strcpy(message,"123456789123456789123456789123456789123456789123456");
	printf("\nEncryption and Decryption: \n");
	res = Send_Elgamal(myPubKey,message,myRand_data,myHidden_data);	
	res = Receive_Elgamal(myPriKey,myRand_data,myHidden_data,plain);
	printf(" Message: %s\n", message);
	printf(" HiddenData: %s\n", myHidden_data);
	printf(" RandData: %s\n", myRand_data);
	printf(" DecryptedMessage: %s\n", plain);

/*	dispose all the variables	*/
	free(myPubKey);
	free(myPriKey);
	free(message);
	free(mySignature);
	free(myRand_data);
	free(myHidden_data);
	free(plain);

	printf("\nTest Over!\n");

	return 1;
}



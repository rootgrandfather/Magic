 /*c-program of block cipher IDEA*/
#include<windows.h>
#include<stdio.h>
#include<process.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#define  maxim 65537L
#define fuyi 65536L
#define one 65535L
#define round 8
#define buf_size 8
# define SIZE 8
#define swap_byte(x,y) t = *(x); *(x) = *(y); *(y) = t

int WINAPI DLLEntryPoint (HINSTANCE hDLL, DWORD dwReason, LPVOID Reserved);
int deideaFile(char *,char *,unsigned char *);
int enideaFile(char *,char *,unsigned char *);
void deideaString(unsigned char *keysession,unsigned char * inputdata,unsigned long inlength,unsigned char *outdata,unsigned long *outlength);
void enideaString(unsigned char *keysession,unsigned char * inputdata,unsigned long inlength,unsigned char *outdata,unsigned long *outlength);
		 /* encryption algorithm */
void cip(unsigned [],unsigned [],unsigned Z[7][10]);

	/* generate encryption subkeys Z's */
void key(unsigned  uskey[8], unsigned int Z[7][10]);

	 /* compute decryption subkeys DK's */
void de_key(unsigned int Z[7][10], unsigned int DK[7][10]);

	/* compute inverse of xin by Euclidean gcd alg. */
unsigned int inv(unsigned int xin);

	/* multiplication using the Low-High algorithm */
unsigned int mul(unsigned  int a,unsigned int  b);

void produce_plaintext(unsigned char text[],unsigned int XX[]);
void produce_miwen(unsigned char text[8],unsigned int YY[4]);

typedef struct rc4_key
{
   unsigned char state[256];
   unsigned char x;
	unsigned char y;
} rc4_key;


void prepare_key(unsigned char key_data_ptrp[256], int key_data_len, rc4_key *key)
{

  unsigned char t;
  unsigned char index1;
  unsigned char index2;
  unsigned char state[256];
  unsigned char key_data_ptr[256];
  int  counter;
  int i;
/* strcpy(key_data_ptr,key_data_ptrp);*/
  for(i=0;i<key_data_len;i++)
	/*state[i] = key.state[i];*/
		  key_data_ptr[i]=key_data_ptrp[i];
  for(i=0;i<256;i++)
	state[i] = key->state[i];
  for(counter = 0; counter < 256; counter++)
	state[counter] =(char) counter;
  key->x = 0;
  key->y = 0;
  index1 = 0;
  index2 = 0;
  for(counter = 0; counter < 256; counter++)
  {
	 index2 = (key_data_ptr[index1] +state[counter] + index2) % 256;
	 swap_byte(state+counter, state+index2);
	 index1 = (index1 + 1) % key_data_len;
  }
  for(i=0;i<256;i++)
	key->state[i] =state[i];

}


 void rc4(unsigned char buffer_ptr[8], int buffer_len, rc4_key *key)
{
  unsigned char t;
  unsigned char x;
  unsigned char y;
  unsigned char state[256];
  unsigned char xorIndex;
  short counter;
  int i;
  x = key->x;
  y = key->y;
  for(i=0;i<256;i++)
		state[i] = key->state[i];
  for(counter = 0; counter < buffer_len; counter++)
  {
	 x = (x + 1) % 256;
	 y = (state[x] + y) % 256;
	 swap_byte(state+x, state+y);
	 xorIndex = (state[x] + state[y]) % 256;
	 buffer_ptr[counter] ^= state[xorIndex];
  }

  for(i=0;i<256;i++)
		key->state[i] = state[i];

  key->x = x;
  key->y = y;
}


void RC4RC4(unsigned char lSourcestr[],unsigned char* lSessionkey)
{
  unsigned char seed[256];
  unsigned char data[256];
  unsigned char buf[buf_size];
  unsigned char digit[5];
  long hex;int rd;
  int i;
  int   n;
  rc4_key key;
  int abcd;
  abcd=strlen((char *)lSessionkey);
  strcpy((char *)data,(char *)lSessionkey);
  data[abcd]='\0';
  n = strlen((char *)data);

  if (n&1)
		{
		 strcat((char *)data,"0");
		 n++;
		}
  n/=2;

  strcpy((char *)digit,"AA");
  for (i=0;i<n;i++)
		 {
			digit[2] = data[i*2];
			digit[3] = data[i*2+1];
			hex=0xaa10;
			seed[i] = (char)hex;
		 }

  prepare_key(seed,n,&key);
  rd=strlen((char *)lSourcestr);
  strcpy((char *)buf,(char *)lSourcestr);
  buf[rd]='\0';
  rd=strlen((char *)buf);

  rc4(buf,rd,&key);
  buf[rd]='\0';
  strcpy((char *)lSourcestr,(char *)buf);
  return;
}

  void produce_plaintext(unsigned char text[8],unsigned int XX[4])
  {
	int i;
	for(i=0;i<4;i++)
		{
		XX[i]=text[2*i];
		XX[i]=(XX[i]<<8)+text[2*i+1];
		}
	}

  void produce_miwen(unsigned char text[8],unsigned int YY[4])
  {
	int i;
	for(i=0;i<4;i++)
		{

		YY[i]=(unsigned int)text[2*i];
		YY[i]=(YY[i]<<8)+text[2*i+1];

		}

	}

	 /* encryption algorithm */
  void cip(unsigned int in[4],unsigned  int out[4],unsigned int Z[7][10])
  {

  unsigned int r,x1,x2,x3,x4,kk,t1,t2,a;
  x1=in[0];
  x2=in[1];
  x3=in[2];
  x4=in[3];
  for (r=1;r<=8;r++) /* the round function */
	{
	/* the group operation on 64-bits block */
	x1=mul(x1,Z[1][r]);x4=mul(x4,Z[4][r]);
	x2=(x2+Z[2][r])&one;x3=(x3+Z[3][r])&one;
	/* the function of the MA structure */
	kk=mul(Z[5][r],(x1^x3));
	t1=mul(Z[6][r],(kk+(x2^x4))&one);
	t2=(kk+t1)&one;
	/* the involutary permutation PI */
	x1=x1^t1;x4=x4^t2;a=x2^t2;x2=x3^t1;x3=a;
	}
	/* the output transformation */
	out[0]=mul(x1,Z[1][round+1]);
	out[3]=mul(x4,Z[4][round+1]);
	out[1]=(x3+Z[2][round+1])&one;
	out[2]=(x2+Z[3][round+1])&one;
	}
	  /* multiplication using the Low-High algorithm */
	unsigned int  mul(unsigned  int a,unsigned  int b)
	{
	long int p;
	long unsigned q;
	if(a==0) p=maxim-b;
	else if(b==0) p=maxim-a;else
		 {q=(unsigned long)a*(unsigned long)b;
		 p=(q&one)-(q>>16);if(p<=0) p=p+maxim;
		 }
		 return(unsigned ) (p&one);
 }
		/* compute inverse of xin by Euclidean gcd alg. */
 unsigned inv (unsigned  xin)
 {
 long n1,n2,q,r,b1,b2,t;
 if(xin==0) b2=0;
 else
 {n1=maxim;n2=xin;b2=1;b1=0;
	do {r=(n1%n2);q=(n1-r)/n2;
	  if(r==0) {if(b2<0) b2=maxim+b2;}
	  else {n1=n2;n2=r;t=b2;b2=b1-q*b2;b1=t;}
	  } while(r!=0);
	  }
	  return(unsigned long int )b2;
	  }
	  /* generate encryption subkeys Z's */

void key(unsigned int uskey[8],unsigned int Z[7][10])
  {
  unsigned  int S[54];
  int i,j,r;
  for(i=0;i<8;i++) S[i]=uskey[i];
	/*shifts*/
	 for(i=8;i<54;i++)
	 {
	 if((i+2)%8==0)/*forS[14],S[22],...*/
	 S[i]=((S[i-7]<<9)^(S[i-14]>>7))&one;
	 else if ((i+1)%8==0)/*for S[15],S[23],...*/
		S[i]=((S[i-15]<<9)^(S[i-14]>>7))&one;
	 else
		S[i]=((S[i-7]<<9)^(S[i-6]>>7))&one;
		}
		/*get subkeys*/
	for(r=1;r<=round+1;r++) for(j=1;j<7;j++)
	Z[j][r]=S[6*(r-1)+j-1];
		}

  /* compute decryption subkeys DK's */
 void de_key(unsigned int Z[7][10],unsigned  int  DK[7][10])
 {
  int j;
  for(j=1;j<=round+1;j++)
	{ DK[1][round-j+2]=inv(Z[1][j]);
	 DK[4][round-j+2]=inv(Z[4][j]);
	 if((j==1)|(j==round+1))
	 {
	  DK[2][round-j+2]=(fuyi-Z[2][j])&one;
	  DK[3][round-j+2]=(fuyi-Z[3][j])&one;
	  }
	  else
	  {
	 DK[2][round-j+2]=(fuyi-Z[3][j])&one;
	DK[3][round-j+2]=(fuyi-Z[2][j])&one;
		}
		}


 for(j=1;j<=round+1;j++)
 {DK[5][round+1-j]=Z[5][j];DK[6][round+1-j]=Z[6][j];}
	  }


 int enideaFile(char *sourcefile,char *desfile,unsigned char *sessionkey)
{
	FILE *fp1,*fp2;
	unsigned	char buf1[9],buf2[9];
	int numread;
   int i;
	unsigned  int Z[7][10],XX[4],YY[4];
	unsigned  int  uskey[8];
	unsigned char initkey[]="12345678";
	unsigned char *inputkey;
	if((fp1=fopen(sourcefile,"rb"))==NULL){
	fprintf(stderr,"Can't open the plaintext_file");
	  return 10;
	  }

	if((fp2=fopen(desfile,"wb"))==NULL){
		fprintf(stderr,"Can't open the ciphertext_file");
		return 15;
	  }


	inputkey=sessionkey;
	for(i=0;i<8;i++)
		uskey[i]=(((unsigned int)inputkey[2*i]<<8)^((unsigned int)inputkey[2*i+1]));
	key(uskey,Z);  /*generate encryption subkeys Z[i][r]*/

	do{
		  numread=fread(buf1,1,SIZE,fp1);
		  if(numread==0) break;
		  if(numread!=SIZE)
		{
			buf1[numread]='\0';
			RC4RC4(buf1,initkey);
			buf1[numread]='\0';
			fwrite(buf1,1,numread,fp2);
			break;
		}
		buf1[8]='\0';

 /* Ming wen in XX[1..5] */
	produce_plaintext(buf1,XX);
	cip(XX,YY,Z);
	for (i=0;i<4;i++)
		 {
		  buf2[2*i]=(YY[i]>>8)&0xff;
		  buf2[2*i+1]=YY[i]&0xff;
		 }

	 buf2[8]='\0';
	 fwrite(buf2,1,SIZE,fp2);
	}while(numread!=0);
	fclose(fp1);
	fclose(fp2);
	return 1;
	}

 int deideaFile(char *sourcefile,char *desfile,unsigned char *sessionkey)
  {
	FILE *fp1,*fp2;
	unsigned char buf1[9],buf2[9];
	int numread;
	int i;
	unsigned  int Z[7][10],DK[7][10],TT[4],YY[4];
	unsigned  int  uskey[8];
	unsigned char initkey[]="12345678";
	unsigned char *inputkey;
	if((fp1=fopen(sourcefile,"rb"))==NULL){
	fprintf(stderr,"Can't open the ciphertext_file");
	return 10;
	  }

	if((fp2=fopen(desfile,"wb"))==NULL){
		fprintf(stderr,"Can't open the deplaintext_file");
		return 15;
	  }

	  inputkey=sessionkey;
	  for(i=0;i<8;i++)
			uskey[i]=(((unsigned int)inputkey[2*i]<<8)^((unsigned int)inputkey[2*i+1]));

	 key(uskey,Z);  /*generate encryption subkeys Z[i][r]*/
	 de_key(Z,DK);/* compute decryption subkeys DK[i][r]*/

	do{
		  numread=fread(buf1,1,SIZE,fp1);
		  if(numread==0) break;
		  if(numread!=SIZE)
		{
			 buf1[numread]='\0';
			 RC4RC4(buf1,initkey);
			 buf1[numread]='\0';
			fwrite(buf1,1,numread,fp2);
			break;
		}
		buf1[8]='\0';

 /* Ming wen in XX[1..5] */
	produce_miwen(buf1,YY);
	cip(YY,TT,DK);
	for (i=0;i<4;i++)
		 {
		  buf2[2*i]=(TT[i]>>8)&0xff;
		  buf2[2*i+1]=TT[i]&0xff;
		 }

	 buf2[8]='\0';
	 fwrite(buf2,1,SIZE,fp2);
	}while(numread!=0);
	fclose(fp1);
	fclose(fp2);
	return 1;
	}

void DeMemory(unsigned char* a)
{ free(a);
}


void enideaString(unsigned char *keysession,unsigned char * inputdata,unsigned long inlength,unsigned char *outdata,unsigned long *outlength)
 {
	//FILE *fp1,*fp2;
	//outputdata="\0";
	unsigned char buf1[9],buf2[9];
	int i,j,k,n;
	unsigned  int Z[7][10],XX[4],YY[4];

	unsigned  int  uskey[8];
	unsigned char initkey[]="12345678";
	unsigned char *inputkey;
	unsigned char *temp,*tempoutdata;

	inputkey=keysession;
	for(i=0;i<8;i++)
		uskey[i]=(((unsigned int)inputkey[2*i]<<8)^((unsigned int)inputkey[2*i+1]));
	key(uskey,Z);  /*generate encryption subkeys Z[i][r]*/


   //	outdata=(unsigned char *)malloc(inlength*sizeof(unsigned long));
	*outlength=inlength;
	k=inlength/8;
	if(k==0)
	{
	memcpy(outdata,inputdata,strlen((char *)inputdata));
	RC4RC4(outdata,initkey);
	}
	else
	{
	temp=inputdata;
	tempoutdata=outdata;
	 for(i=1;i<=k;i++)
	   {
		  for(j=0;j<8;j++)
		{
		buf1[j]=*(temp+j);
		}
		buf1[8]='\0';
		produce_plaintext(buf1,XX);
		cip(XX,YY,Z);
		for (n=0;n<4;n++)
		 {
		  buf2[2*n]=(YY[n]>>8)&0xff;
		  buf2[2*n+1]=YY[n]&0xff;
		 }

		temp+=8 ;
		memcpy(tempoutdata,buf2,8);
		tempoutdata+=8;
//		strcpy(outdata,buf1);
		}
	memcpy(tempoutdata,temp,strlen((char *)temp)+1);
	RC4RC4(tempoutdata,initkey);
	}
	//  return outdata;
  }

 void deideaString(unsigned char *keysession,unsigned char * inputdata,unsigned long inlength,unsigned char *outdata,unsigned long *outlength)
 {
   unsigned char buf1[9],buf2[9];
	int i,k,j,n;
	unsigned  int Z[7][10],DK[7][10],TT[4],YY[4];
	unsigned  int  uskey[8];
	unsigned char initkey[]="12345678";
	unsigned char *inputkey;


	unsigned char *temp,*tempoutdata;


	//outdata=(unsigned char *)malloc(inlength*sizeof(unsigned long));
	*outlength=inlength;
	inputkey=keysession;
	for(i=0;i<8;i++)
		uskey[i]=(((unsigned int)inputkey[2*i]<<8)^((unsigned int)inputkey[2*i+1]));
	 key(uskey,Z);  /*generate encryption subkeys Z[i][r]*/
	 de_key(Z,DK);/* compute decryption subkeys DK[i][r]*/

	k=inlength/8;
	if(k==0)
	{ memcpy(outdata,inputdata,strlen((char *)inputdata));
	  RC4RC4(outdata,initkey);
	}
	else
	{temp=inputdata;
	 tempoutdata=outdata;
	 for(i=1;i<=k;i++)
	   {
		  for(j=0;j<8;j++)
		{
		buf1[j]=*(temp+j);
		}
		temp+=8;
		produce_miwen(buf1,YY);
		cip(YY,TT,DK);
		for (n=0;n<4;n++)
		 {
		  buf2[2*n]=(TT[n]>>8)&0xff;
		  buf2[2*n+1]=TT[n]&0xff;
		 }
		memcpy(tempoutdata,buf2,8);
		tempoutdata+=8;
		}
	memcpy(tempoutdata,temp,strlen((char *)temp)+1);
	RC4RC4(tempoutdata,initkey);
	}
  //	return outdata;
  }


 int WINAPI DLLEntryPoint (HINSTANCE hDLL, DWORD dwReason, LPVOID Reserved)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{

			break;
		}
	case DLL_PROCESS_DETACH:
		{

			break;
		}
	}
	return TRUE;
}
/*
  void main(void)
{
 unsigned char  revalue1[500], revalue2[500];
 //revalue1 is the ciphertext, revalue2 is the deplaintext
 unsigned char key[]="123456789abcdefg";
 //Key is the session key, you can select any 8 bytes as the key
 unsigned char in[]="I am a student. Now i am studing in Haerbin!!!!";
 //in is the plaintext
 unsigned long outlength;
 printf("Now start!!!!\n");
enideaString(key,in,strlen(in),revalue1,&outlength);
printf("%s\n",revalue1);

deideaString(key,revalue1,outlength,revalue2,&outlength);
printf("\n%s\n",revalue2);
}
*/
/* void main(void)
{
 int revalue1,revalue2;
 char valuere[20];
 unsigned char key[]="12345678abcdefgh";

 //MessageBox(NULL, "Press any key to start","Start",MB_OK|MB_ICONINFORMATION);
 printf("Now start!!!!\n");
 revalue1=enidea("c:\\feng\\plaintxt.txt","c:\\feng\\miwen.txt",key);
  revalue2=deidea("c:\\feng\\miwen.txt","c:\\feng\\jiemi.txt",key);
 if(revalue1==1)
   MessageBox(NULL, "The End!","OK!",MB_OK|MB_ICONINFORMATION);
	printf("Jiami the end. OK!\n");
 else
  {itoa(revalue1,valuere,10);
   //	/*MessageBox(NULL, valuere, "Mistake",MB_OK|MB_ICONINFORMATION);
	printf("Jiami Mistake.\n");
  }
  if(revalue2==1)
   //MessageBox(NULL, "The End!","OK!",MB_OK|MB_ICONINFORMATION);
	 printf("Jiemi the end. OK!\n");
  else
  {itoa(revalue2,valuere,10);
   //	MessageBox(NULL, valuere, "Mistake",MB_OK|MB_ICONINFORMATION);
	printf("jiemi Mistake.\n");

  }
}    */

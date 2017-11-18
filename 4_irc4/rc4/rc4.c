
#include "rc4.h"

void prepare_key(unsigned char key_data_ptrp[256], int key_data_len, rc4_key *key)
{

  unsigned char t;
  unsigned char index1;
  unsigned char index2;
  unsigned char state[256];
  unsigned char key_data_ptr[256];
  int  counter;
  int i;
//  strcpy(key_data_ptr,key_data_ptrp);
  for(i=0;i<key_data_len;i++)
  	//state[i] = key.state[i];
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
  //unsigned char  buffer_ptr[256];
  short counter;
  int i;
  //strcpy( buffer_ptr,buffer_ptrp);
  x = key->x;
  y = key->y;
  for(i=0;i<256;i++)
   	state[i] = key->state[i];
  // state = &key->state[0];
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
  // strcpy(buffer_ptrp,buffer_ptr);
}


int RC4File(char* filename1,char* filename2,char* session)
{
  unsigned char seed[256];
  char data[256];
  unsigned char buf[buf_size];
  char digit[5];
  long hex;int rd;
  int i;
  int   n;
  rc4_key key;

  FILE *fp1,*fp2;

  //char * init_key;

  int abcd;

  //Input File Name:(include whole path)
  //filename1="c:\\lxdvc\\test.dat";

  if((fp1=fopen(filename1,"rb"))==NULL)
      {
	   return 15;
      }

  //filename2="c:\\lxdvc\\d.dat";

  if((fp2=fopen(filename2,"wb"))==NULL)
      {
	   return 20;
      }


  abcd=strlen(session);
  strcpy(data,session);
  data[abcd]='\0';
  //for(i=0;i<8;i++)
  //   data[i]=i+1;
  //data[i]='\0';
  n = strlen(data);

  if (n&1)
      {
       strcat(data,"0");
       n++;
	  }
  n/=2;

  strcpy(digit,"AA");
  for (i=0;i<n;i++)
	    {
	      digit[2] = data[i*2];
		  digit[3] = data[i*2+1];
	      //hex=0xaa10;
	      sscanf(digit,"%x",&hex);
	      seed[i] = (char)hex;
	    }

  prepare_key(seed,n,&key);
  rd = fread(buf,1,buf_size,fp1);
  while (rd>0)
	      {
	   rc4(buf,rd,&key);
	   fwrite(buf,1,rd,fp2);
	   rd = fread(buf,1,buf_size,fp1);
		  }
  fclose(fp1);
  fclose(fp2);
  return 1;
}

void RC4String(char *keysession,unsigned char* inputdata,unsigned long inputlength,unsigned char *outputdata,unsigned long *outlength)
{
  unsigned char seed[256];
  unsigned char data[256];
  unsigned char buf[buf_size];
  char digit[5];
  long hex;
  int i,j;
  int   n;
  rc4_key key;
  unsigned char *temp,*tempoutdata;
  int k=inputlength/8;

  int abcd;

  abcd=strlen(keysession);
  strcpy(data,keysession);
  data[abcd]='\0';

  n = strlen(data);

  if (n&1)
	  {
	   strcat(data,"0");
	   n++;
	  }
  n/=2;

  strcpy(digit,"AA");
  for (i=0;i<n;i++)
		{
		  digit[2] = data[i*2];
		  digit[3] = data[i*2+1];
		  sscanf(digit,"%x",&hex);
		  seed[i] = (char)hex;
		}

  prepare_key(seed,n,&key);

  temp=inputdata;
  tempoutdata=outputdata;
  *outlength=inputlength;
  for(i=1;i<=k;i++)
   {
	  for(j=0;j<8;j++)
		{
		buf[j]=*(temp+j);
		}
	  temp+=8 ;
	  rc4(buf,8,&key);
	  memcpy(tempoutdata,buf,8);
	  tempoutdata+=8;
  }
  memcpy(tempoutdata,temp,strlen((char *)temp)+1);
  rc4(tempoutdata,strlen((char *)temp)+1,&key);

//  return outputdata;
}


/*void main(void)
{
 unsigned char  revalue1[1000], revalue2[1000];
 //revalue1 is the ciphertext, revalue2 is the deplaintext
 unsigned char key[]="123456789abcdef";
 //Key is the session key, you can select any 8 bytes as the key
 unsigned char in[]="I am a student. Now i am studing in Haerbin!!!!";
 //in is the plaintext
 unsigned long outlength;
 printf("Now start!!!!\n");

 int i;
 for(i=0;i<1000;i++)
  revalue1[i]=0;

RC4String(key,in,strlen(in),revalue1,&outlength);
printf("%s\n",revalue1);

RC4String(key,revalue1,outlength,revalue2,&outlength);
printf("\n%s\n",revalue2);
}*/


BOOL WINAPI DllEntryPoint(HINSTANCE hDll,DWORD dwReason,
						  LPVOID Reserved)
{
switch (dwReason)
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
 int lxdyf;
 char yflxd[20];

// MessageBox(NULL, "Press any key to start","Start",
  //			 MB_OK|MB_ICONINFORMATION);
 lxdyf=RC4RC4("c:\\feng\\plaintxt.txt","c:\\feng\\rc4.dat","1234567");
 if(lxdyf==1)
 ; // MessageBox(NULL, "The End!","OK!",MB_OK|MB_ICONINFORMATION);
 else
  {itoa(lxdyf,yflxd,10);
  // MessageBox(NULL, yflxd, "Mistake",MB_OK|MB_ICONINFORMATION);
  }
}*/


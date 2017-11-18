#include "myCrySubstitude.h"

int Encrypt(unsigned char* str,unsigned char* code)
{
	if(str==NULL || code==NULL)
		return CRYPT_ERROR;

	int str_length = 0;

	int i = 0;

	while(str[i]!='\0')
	{
		str_length++;
		i++;
	}

	if(str_length>1024)
	{
		printf("���ĵĳ��ȳ���1024λ");
		return CRYPT_ERROR;
	}
	else if(code[1] != '\0')
	{
		printf("��Կ�����ַ�����һλ");
		return CRYPT_ERROR;
	}
	
	if(ToLowerCase(str) != 0)
	{
		printf("�����ַ���ת��Сд�ַ�����");
		return CRYPT_ERROR;
	}

	if(ToLowerCase(code) != 0)
	{
		printf("��Կ�ַ���ת��Сд�ַ�����");
		return CRYPT_ERROR;
	}

	int key = code[0]-97;
	for(i=0;i<str_length;i++)
	{
		str[i]+=key;
		if(str[i]>122)
			str[i]-=26;
	}

	return CRYPT_OK;
}


int Decrypt(unsigned char* str,unsigned char* code)
{
	if(str==NULL || code==NULL)
		return DECRYPT_ERROR;

	int str_length = 0;

	int i = 0;

	while(str[i]!='\0')
	{
		str_length++;
		i++;
	}

	if(str_length>1024)
	{
		printf("���ĵĳ��ȳ���1024λ");
		return CRYPT_ERROR;
	}
	else if(code[1] != '\0')
	{
		printf("��Կ�����ַ�����һλ");
		return CRYPT_ERROR;
	}
	
	if(ToLowerCase(str) != 0)
	{
		printf("�����ַ���ת��Сд�ַ�����");
		return CRYPT_ERROR;
	}

	if(ToLowerCase(code) != 0)
	{
		printf("��Կ�ַ���ת��Сд�ַ�����");
		return CRYPT_ERROR;
	}

	int key = code[0]-97;
	for(i=0;i<str_length;i++)
	{
		str[i]-=key;
		if(str[i]<97)
			str[i]+=26;
	}

	return CRYPT_OK;
	
	
}

int ToLowerCase(unsigned char* str)
{
	if(str == NULL)
		return 0;

	int i = 0;

	for(i=0;str[i]!='\0';i++)
	{
		if(str[i]>64&&str[i]<91)
			str[i]+=32;
		else if(str[i]>96&&str[i]<123)
			;
		else
		{
			printf("�����˷�Ӣ���ַ�");
			return 1;
		}
	}
	return 0;

}
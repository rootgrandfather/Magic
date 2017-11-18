/*
 *   I/O routines 1. 
 *   mrio1.c
 */

#include <stdio.h>
#include "miracl.h"


int instr( flash x,char *string)
{  /*  input a big number       *
    *  returns length in digits */
    int i,ipt,n,s,e;
    int ch;

    if (mr_mip->apbase==0 || mr_mip->apbase>256)			
        return 0;

    zero(x);
    if (mr_mip->INPLEN==0)
    { /* inputting ASCII bytes */

        forever
        { /*  get input length  */
            ch=(unsigned char)string[mr_mip->INPLEN];
            if (ch=='\0') 
				break;
            if (mr_mip->apbase<=60 && ch=='\n') 
				break;
            mr_mip->INPLEN++;
            if (string==mr_mip->IOBUFF && mr_mip->INPLEN>=mr_mip->IOBSIZ)				
                return 0;
        }
    }
    else
    { /* inputting BINARY bytes */
        if (string==mr_mip->IOBUFF && mr_mip->INPLEN>=mr_mip->IOBSIZ)				
            return 0;
    }
    n=0;
    s=PLUS;
    e=0;

    if (mr_mip->INPLEN>0 && mr_mip->apbase<=60)
    { /* skip leading blanks and check sign */

        while (string[e]==' ') e++;
        if (string[e]=='-')
        { /* check sign */
             s=MINUS;
             e++;
        }
        if (string[e]=='+') e++;
    }
    for (i=mr_mip->INPLEN-1;i>=e;i--)
    {
        ch=(unsigned char)string[i];
        if (mr_mip->apbase<=60)
        { /* check for slash or dot and convert character to number */

            ch+=80;
            if (ch>127 && ch<138) ch-=128;
            if (mr_mip->apbase<=16)
            {
                if (ch>144 && ch<171) ch-=135;
                if (ch>176 && ch<203) ch-=167;
            }
            else
            {
                if (ch>144 && ch<171) ch-=135;
                if (ch>176 && ch<203) ch-=141;
            }
        }
         
        if ((mr_small)ch>=mr_mip->apbase)			
            return 0;
        n++;
        putdig( ch,x,n);
    }
    ipt=mr_mip->INPLEN;
    mr_mip->INPLEN=0;
	if (x->len!=0)
	{
		if (s<0) x->len|=MR_MSBIT;
		else     x->len&=MR_OBITS;
	}
    mr_lzero(x);

    return ipt;
}

int otstr( flash x,char *string)
{  /*  output a big number  */
    int s,i,n,ch,rp,nd,m;

    mr_unsign32 lx;
    BOOL done;


    if (mr_mip->apbase==0 || mr_mip->apbase>256)			
        return 0;

    n=0;
	s = MINUS;
	if ((x->len&(MR_MSBIT))==0) s=PLUS;
	if (x->len!=0)
		x->len&=MR_OBITS;

    lx = x->len;
    if (lx==0 && mr_mip->apbase<=60)
    {
        string[0]='0';
        string[1]='\0';
        return 1;
    }
    rp=0;
    if (s==MINUS && mr_mip->apbase<=60)
    {
        string[n]='-';
        n++;
    }

    copy(x,mr_mip->w6);
    done=TRUE;

    forever
    {
/* returns number of digits in x */
		if (mr_mip->w6->len==0) return 0;

		nd=(int)(mr_mip->w6->len&(MR_OBITS))*mr_mip->pack;
		while (getdig( mr_mip->w6,nd)==0)	nd--;

        m=nd;
        if (rp>m) m=rp;
        for (i=m;i>0;i--)
        { 
            if (string==mr_mip->IOBUFF && n>=mr_mip->IOBSIZ-3)			
                return n;

            if (i>nd) ch='0';
            else
            {
                ch=getdig( mr_mip->w6,i);
                mr_mip->check=OFF;
                putdig( 0,mr_mip->w6,i);
                mr_mip->check=ON;
                if (mr_mip->apbase<=60)
                { /* convert number to character */
                    ch+=48;
                    if (ch>=58) ch+=7;
                    if (ch>=91) ch+=6;
                }
            }
            if (i<rp && mr_mip->apbase<=60 && ch=='0' && size(mr_mip->w6)==0) break;
            string[n]=(char)ch; 
            n++;
        }
        if (done) break;

    }
/* Append a trailing 0 - it may be printable ascii text */    
    string[n]='\0';
	if (x->len!=0)
	{
		if (s<0) x->len|=MR_MSBIT;
		else     x->len&=MR_OBITS;
	}
    return n;
}
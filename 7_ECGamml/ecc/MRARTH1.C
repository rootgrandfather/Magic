/*
 *   Arithmetic routines 1 - multiplying and dividing BIG NUMBERS by  
 *   integer numbers.
 *   mrarth1.c
 */

#include <stdio.h>
#include "miracl.h"


int mr_pmul( big x,mr_small sn,big z)
{ 
    int m,xl;
    mr_unsign32 sx;
    mr_small carry,*xg,*zg;
    mr_large dble;


    if (x!=z)
    {
        zero(z);
        if (sn==0) return 1;
    }
    else if (sn==0)
    {
        zero(z);
        return 1;
    }
    m=0;
    carry=0;
    sx=x->len&MR_MSBIT;
    xl=(int)(x->len&MR_OBITS);

    if (mr_mip->base==0) //?
    {
        xg=x->w; zg=z->w;
        for (m=0;m<xl;m++)
        {
            dble=(mr_large)x->w[m]*sn+carry;
            carry=MR_TOP(dble);
            z->w[m]=(mr_small)dble;
        }

        if (carry>0)
        {
            m=xl;
            if (m>=mr_mip->nib && mr_mip->check)
                return 0;
            z->w[m]=carry;
            z->len=m+1;
        }
        else z->len=xl;
    }
    else while (m<xl || carry>0)
    { /* multiply each digit of x by n */ 
    
        if (m>mr_mip->nib && mr_mip->check)
            return 0;

        carry=muldiv(x->w[m],sn,carry,mr_mip->base,&z->w[m]);

        m++;
        z->len=m;
    }
    if (z->len!=0) z->len|=sx;
	return 1;
}

void premult( big x,int n,big z)
{ /* premultiply a big number by an int z=x.n */

    if (n==0)  /* test for some special cases  */
    {
        zero(z);
        return;
    }
    if (n==1)
    {
        copy(x,z);
        return;
    }
    if (n<0)
    {
        n=(-n);
        mr_pmul( x,(mr_small)n,z);
        if (z->len!=0) z->len^=MR_MSBIT;
    }
    else mr_pmul( x,(mr_small)n,z);
}


mr_small mr_sdiv( big x,mr_small sn,big z)
{
    int i,xl;
    mr_small sr,*xg,*zg;
    mr_large dble;

    sr=0;
    xl=(int)(x->len&MR_OBITS);
    if (x!=z) zero(z);
    if (mr_mip->base==0) 
    {
        xg=x->w; zg=z->w;

        for (i=xl-1;i>=0;i--)
        {
            MR_BOT(dble)=x->w[i];
            MR_TOP(dble)=sr;
            z->w[i]=(mr_small)(dble/sn);
            sr=(mr_small)(dble-(mr_large)z->w[i]*sn);
		}
    }
    else for (i=xl-1;i>=0;i--)
	{ /* divide each digit of x by n */

        dble=(mr_large)sr*mr_mip->base+x->w[i];
        z->w[i]=(mr_small)MR_LROUND(dble/sn);
        sr=(mr_small)(dble-(mr_large)z->w[i]*sn);
	}
    z->len=x->len;
    mr_lzero(z);
    return sr;
}
         
int subdiv( big x,int n,big z)
{  /*  subdivide a big number by an int   z=x/n  *
    *  returns int remainder                     */ 
    mr_unsign32 sx;
    int r;

    if (n==0) 	
		return 0;

    if (n==1) /* special case */
    {
        copy(x,z);
        return 0;
    }
    sx=(x->len&MR_MSBIT);

    if (n<0)
    {
        n=(-n);

        r=(int)mr_sdiv( x,(mr_small)n,z);

        if (z->len!=0) z->len^=MR_MSBIT;
    }
    else r=(int)mr_sdiv( x,(mr_small)n,z);

    if (sx==0) return r;
    else       return (-r);
}

int remain( big x,int n)
{ /* return integer remainder when x divided by n */
    int r;
    mr_unsign32 sx;

    sx=(x->len&MR_MSBIT);
    if (n==2 && MR_REMAIN(mr_mip->base,2)==0)
    { /* fast odd/even check if base is even */
        if (MR_REMAIN(x->w[0],2)==0) return 0;
        else
        {
            if (sx==0) return 1;
            else       return (-1);
        } 
    }
    copy(x,mr_mip->w0);
    r=subdiv( mr_mip->w0,n,mr_mip->w0);
    return r;
}

int bytes_to_big( int len,char *ptr,big x)
{ /* convert len bytes into a big           *
   * The first byte is the Most significant */
    int i,j,m,n,r;
    unsigned int dig;
    unsigned char ch;
    mr_small wrd;

    zero(x);

    if (len<=0)
        return 1;

/* remove leading zeros.. */
    while (*ptr==0) 
    {
        ptr++; len--;
        if (len==0) 
            return 1;
    }

    if (mr_mip->base==0)
    { /* pack bytes directly into big */
        m=MIRACL/8;  //MIRACL=32;  m=4
        n=len/m;     //n=len/4
        r=len%m;
        if (r!=0)
        {
            n++;
            wrd=(mr_small)0;  
            for (j=0;j<r;j++) {wrd<<=8; wrd|=(unsigned char)*ptr++; }
        }
        x->len=n;
        if (n>mr_mip->nib && mr_mip->check)			
            return 0;
        if (r!=0) 
        {
            n--;
            x->w[n]=wrd;   //w[n] of big x is the beginning of string,the length of w[n] is 0,1,2,3.
        }      
        for (i=n-1;i>=0;i--)
        {
            for (j=0;j<m;j++) { wrd<<=8; wrd|=(unsigned char)*ptr++; }
            x->w[i]=wrd;
        }
        mr_lzero(x);     /* needed */   // strip the zeros of the beginning of x->w[i].
    }
    else
    {
        for (i=0;i<len;i++)
        {
			mr_pmul( x,(mr_small)3,x);
            ch=(unsigned char)ptr[i];
            dig=ch;  
			convert( (int)dig,mr_mip->w0);
			myselect( x,PLUS,mr_mip->w0,x);
        }
    }
	return 1;
} 

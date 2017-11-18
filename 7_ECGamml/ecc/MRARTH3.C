/*
 *   Arithmetic routines 3 - simple powers and roots
 *   mrarth3.c
 */

#include <stdio.h>
#include "miracl.h"


#define mr_abs(x)  ((x)<0? (-(x)) : (x))

int logb2( big x)
{ /* returns number of bits in x */
    int xl,lg2;

    if (size(x)==0) return 0;


    copy(x,mr_mip->w0);
    if (mr_mip->w0->len!=0) 
		mr_mip->w0->len&=MR_OBITS;
    lg2=0;
    xl=(int)mr_mip->w0->len;
    if (mr_mip->base==mr_mip->base2)
    {
        lg2=mr_mip->lg2b*(xl-1);
        mr_shift( mr_mip->w0,1-xl,mr_mip->w0);
    }
    else while (mr_mip->w0->len>1)
    {

        mr_sdiv( mr_mip->w0,mr_mip->base2,mr_mip->w0);

        lg2+=mr_mip->lg2b;
    }
    while (mr_mip->w0->w[0]>=1)
    {
        lg2++;
        mr_mip->w0->w[0]/=2;
    }
    return lg2;
}

void expint( int b,int n,big x)
{ /* sets x=b^n */
    int i,r,p;
    unsigned int bit,un;
	mr_small shift;

    convert( 1,x);
    if (n==0) return;

    if (n<0)
			
    {
        return;
    }
    if (b==2)
    {
        r=n/mr_mip->lg2b;
        p=n%mr_mip->lg2b;
        if (mr_mip->base==mr_mip->base2)
        {
            mr_shift( x,r,x);
			if (p>0) x->w[x->len-1]<<=p;
			else x->w[x->len-1]>>=p;
        }
        else
        {
            for (i=1;i<=r;i++)
                mr_pmul( x,mr_mip->base2,x);
			if (p>0) shift =(mr_small)1<<p;
			else shift=(mr_small)1>>p;
	        mr_pmul( x,shift,x);
        }
    }
    else
    {
        bit=1;
        un=(unsigned int)n;
        while (un>=bit) bit<<=1;
        bit>>=1;
        while (bit>0)
        { /* ltr method */
            multiply( x,x,x);
            if ((bit&un)!=0) 
			{
				if (b<0)
				{
					b=(-b);
					mr_pmul( x,(mr_small)b,x);
					if (x->len!=0) x->len^=MR_MSBIT;
				}
				else mr_pmul( x,(mr_small)b,x);
			}
            bit>>=1;
        }
    }
}   

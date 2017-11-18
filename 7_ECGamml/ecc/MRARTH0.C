/*
 *   Arithmetic routines 0 - Add and subtract routines 
 *   mrarth0.c
 */

#include <stdio.h>
#include "miracl.h"


int mr_padd( big x,big y,big z)
{ /*  add two  big numbers, z=x+y where *
   *  x and y are positive              */
    int lx,ly,lz,la,i;
    mr_small carry,psum;
    mr_small *gx,*gy,*gz; 

    lx = (int)x->len;
    ly = (int)y->len;
    
    if (ly>lx)
    {
        lz=ly;
        la=lx;
        if (x!=z) copy(y,z); 
        else la=ly;  
    }
    else
    {
        lz=lx;
        la=ly;
        if (y!=z) copy(x,z);
        else la=lx;
    }
    carry=0;
    z->len=lz;
    gx=x->w; gy=y->w; gz=z->w;
    if (lz<mr_mip->nib || !mr_mip->check) z->len++;
    if (mr_mip->base==0)   //?
		for (i=0;i<la || carry>0;i++)
    { /* add by columns */
        if (mr_mip->check && i>mr_mip->nib)
			return 0;
        psum=gx[i]+gy[i]+carry;
        if (psum>gx[i]) carry=0; //?
        if (psum<gx[i]) carry=1;
        gz[i]=psum;
    }
    else for (i=0;i<la || carry>0;i++)
    { /* add by columns */
        if (i>mr_mip->nib && mr_mip->check)
            return 0;
        psum=gx[i]+gy[i]+carry;
        carry=0;
        if (psum>=mr_mip->base)
        { /* set carry */
            carry=1;
            psum-=mr_mip->base;
        }
        gz[i]=psum;
    }

    if (gz[z->len-1]==0) z->len--;

	gx=gy=gz=NULL;

	return 1;

}

int mr_psub( big x,big y,big z)
{  /*  subtract two big numbers z=x-y      *
    *  where x and y are positive and x>y  */
    int lx,ly,i;
    mr_small borrow,pdiff;
    mr_small *gx,*gy,*gz;

    lx = (int)x->len;
    ly = (int)y->len;
    if (ly>lx)			
        return 0;
    if (y!=z) copy(x,z);
    else ly=lx;
    z->len=lx;
    gx=x->w; gy=y->w; gz=z->w;
    borrow=0;
    if (mr_mip->base==0) for (i=0;i<ly || borrow>0;i++)
    { /* subtract by columns */
        if (i>lx)			
            return 0;
        pdiff=gx[i]-gy[i]-borrow;
        if (pdiff<gx[i]) borrow=0;
        if (pdiff>gx[i]) borrow=1;
        gz[i]=pdiff;
    }
    else for (i=0;i<ly || borrow>0;i++)
    { /* subtract by columns */
        if (i>lx)
            return 0;
        pdiff=gy[i]+borrow;
        borrow=0;
        if (gx[i]>=pdiff) pdiff=gx[i]-pdiff;
        else
        { /* set borrow */
            pdiff=mr_mip->base+gx[i]-pdiff;
            borrow=1;
        }
        gz[i]=pdiff;
    }
    mr_lzero(z);

	gx=gy=gz=NULL;

	return 1;
}

void myselect( big x,int d,big y,big z)
{ /* perform required add or subtract operation */
    int sx,sy,sz,jf,xgty;

	sx=sy=MINUS;
	if ((x->len&(MR_MSBIT))==0) sx=PLUS;
	if ((y->len&(MR_MSBIT))==0) sy=PLUS;
    x->len&=MR_OBITS;  /* force operands to be positive */
    y->len&=MR_OBITS;
    xgty=compare(x,y);
    jf=(1+sx)+(1+d*sy)/2;
    switch (jf)
    { /* branch according to signs of operands */
		case 0:
			mr_padd( x,y,z);
			sz=MINUS;
			break;
		case 1:
			if (xgty<=0)
			{
				mr_psub( y,x,z);
				sz=PLUS;
			}
			else
			{
				mr_psub( x,y,z);
				sz=MINUS;
			}
			break;
		case 2:
			if (xgty>=0)
			{
				mr_psub( x,y,z);
				sz=PLUS;
			}
			else
			{
				mr_psub( y,x,z);
				sz=MINUS;
			}
			break;
		case 3:
			mr_padd( x,y,z);
			sz=PLUS;
			break;
    }
    if (sz<0) z->len^=MR_MSBIT;         /* set sign of result     */
}

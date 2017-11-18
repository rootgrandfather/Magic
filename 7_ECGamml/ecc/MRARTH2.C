/*
 *   Arithmetic routines 2 - multiplying and dividing BIG NUMBERS.
 *   mrarth2.c
 */

#include <stdio.h>
#include "miracl.h"


/* If a number has more than this number of digits, then squaring is faster */

#define SQR_FASTER_THRESHOLD 4


int multiply( big x,big y,big z)
{  /*  multiply two big numbers: z=x.y  */
    int i,xl,yl,j,ti;
    mr_small carry,*xg,*yg,*w0g;
    mr_unsign32 sz;
    mr_large dble;
    big w0;

    if (y->len==0 || x->len==0) 
    {
        zero(z);
        return 1;
    }
    w0=mr_mip->w0;    /* local pointer */

    sz=((x->len&MR_MSBIT)^(y->len&MR_MSBIT));
    xl=(int)(x->len&MR_OBITS);
    yl=(int)(y->len&MR_OBITS);
    zero(w0);
    if (mr_mip->check && xl+yl>mr_mip->nib)			
        return 0;
    if (mr_mip->base==0)
    {
        xg=x->w; yg=y->w; w0g=w0->w; 
        if (x==y && xl>SQR_FASTER_THRESHOLD)    
                             /* extra hassle make it not    */
                             /* worth it for small numbers */
        { /* fast squaring */
            for (i=0;i<xl-1;i++)
            {  /* long multiplication */
                carry=0;
                for (j=i+1;j<xl;j++)
                { /* Only do above the diagonal */
                    dble=(mr_large)x->w[i]*x->w[j]+carry+w0->w[i+j];
                    w0->w[i+j]=(mr_small)dble;
                    carry=MR_TOP(dble);
                }
                w0->w[xl+i]=carry;
            }
            w0->len=xl+xl-1;
            mr_padd( w0,w0,w0);     /* double it */
            carry=0;
            for (i=0;i<xl;i++)
            { /* add in squared elements */
                ti=i+i;
                dble=(mr_large)x->w[i]*x->w[i]+carry+w0->w[ti];
                w0->w[ti]=(mr_small)dble;
                carry=MR_TOP(dble);
                w0->w[ti+1]+=carry;
                if (w0->w[ti+1]<carry) carry=1;
                else                   carry=0;
            }
        }
        else for (i=0;i<xl;i++)
        { /* long multiplication */
            carry=0;
            for (j=0;j<yl;j++)
            { /* multiply each digit of y by x[i] */

                dble=(mr_large)x->w[i]*y->w[j]+carry+w0->w[i+j];
                w0->w[i+j]=(mr_small)dble;
                carry=MR_TOP(dble);
            }
            w0->w[yl+i]=carry;
        }
    }
    else
    {
        if (x==y && xl>SQR_FASTER_THRESHOLD)
        { /* squaring can be done nearly twice as fast */
            for (i=0;i<xl-1;i++)
            { /* long multiplication */
                carry=0;
                for (j=i+1;j<xl;j++)
                { /* Only do above the diagonal */
                   dble=(mr_large)x->w[i]*x->w[j]+w0->w[i+j]+carry;
                   carry=(mr_small)MR_LROUND(dble/mr_mip->base);
                   w0->w[i+j]=(mr_small)(dble-(mr_large)carry*mr_mip->base);
                }
                w0->w[xl+i]=carry;
            }
            w0->len=xl+xl-1;
            mr_padd( w0,w0,w0);     /* double it */
            carry=0;
            for (i=0;i<xl;i++)
            { /* add in squared elements */
                ti=i+i;
                dble=(mr_large)x->w[i]*x->w[i]+w0->w[ti]+carry;
                carry=(mr_small)MR_LROUND(dble/mr_mip->base);
                w0->w[ti]=(mr_small)(dble-(mr_large)carry*mr_mip->base);

                w0->w[ti+1]+=carry;
                carry=0;
                if (w0->w[ti+1]>=mr_mip->base)
                {
                    carry=1;
                    w0->w[ti+1]-=mr_mip->base;
                }
            }
        }
        else for (i=0;i<xl;i++)
        { /* long multiplication */
            carry=0; 
            for (j=0;j<yl;j++)
            { /* multiply each digit of y by x[i] */
                dble=(mr_large)x->w[i]*y->w[j]+w0->w[i+j]+carry;
                carry=(mr_small)MR_LROUND(dble/mr_mip->base);
                w0->w[i+j]=(mr_small)(dble-(mr_large)carry*mr_mip->base);
            }
            w0->w[yl+i]=carry;
        }
    }
    w0->len=(sz|(xl+yl)); /* set length and sign of result */
    mr_lzero(w0);
    copy(w0,z);
	return 1;
}

void divide( big x,big y,big z)
{  /*  divide two big numbers  z=x/y : x=x mod y  *
    *  returns quotient only if  divide(x,y,x)    *
    *  returns remainder only if divide(x,y,y)    */
    mr_small carry,attemp,ldy,sdy,ra,r,d,tst,psum;
    mr_unsign32 sx,sy,sz;
    mr_small borrow,dig,*w0g,*yg;
    mr_large dble;
    int i,k,m,x0,y0,w00;
    big w0;
    BOOL check;
    int len;

    w0=mr_mip->w0;

    if ((x==y)|| (y->len==0))
		return;

    sx=(x->len&MR_MSBIT);   /*  extract signs ... */
    sy=(y->len&MR_MSBIT);
    sz=(sx^sy);
    x->len&=MR_OBITS;   /* ... and force operands to positive  */
    y->len&=MR_OBITS;
    x0=(int)x->len;
    y0=(int)y->len;
    copy(x,w0);
    w00=(int)w0->len;
    if (mr_mip->check && (w00-y0+1>mr_mip->nib))			
        return;
    d=0;
    if (x0==y0)
    {
        if (x0==1) /* special case - x and y are both mr_smalls */
        { 
            d=MR_DIV(w0->w[0],y->w[0]);
            w0->w[0]=MR_REMAIN(w0->w[0],y->w[0]);
            mr_lzero(w0);
        }
        else if (MR_DIV(w0->w[x0-1],4)<y->w[x0-1])
        while (compare(w0,y)>=0)
        {  /* mr_small quotient - so do up to four subtracts instead */
            mr_psub( w0,y,w0);
            d++;
        }
    }
    if (compare(w0,y)<0)
    {  /*  x less than y - so x becomes remainder */
        if (x!=z)  /* testing parameters */
        {
            copy(w0,x);
            if (x->len!=0) x->len|=sx;
        }
        if (y!=z)
        {
            zero(z);
            z->w[0]=d;
            if (d>0) z->len=(sz|1);
        }
        y->len|=sy;
        return;
    }

    if (y0==1)
    {  /* y is int - so use subdiv instead */
        r=mr_sdiv( w0,y->w[0],w0);

        if (y!=z)
        {
            copy(w0,z);
            z->len|=sz;
        }
        if (x!=z)
        {
            zero(x);
            x->w[0]=r;
            if (r>0) x->len=(sx|1);
        }
        y->len|=sy;
        return;
    }
    if (y!=z) zero(z);

    len=(int)(y->len&MR_OBITS);
    if (mr_mip->base==0)
    {
        if ((r=y->w[len-1]+1)==0) d=1;
        else d=muldvm((mr_small)1,(mr_small)0,r,&r);
    }
    else
        d=MR_DIV(mr_mip->base,(mr_small)(y->w[len-1]+1));   
    if (d!=1) mr_pmul( y,d,y);

    check=mr_mip->check;
    mr_mip->check=OFF;

    if (mr_mip->base==0)
    {
        if (d!=1) mr_pmul( w0,d,w0);
        ldy=y->w[y0-1];
        sdy=y->w[y0-2];
        w0g=w0->w; yg=y->w;
        for (k=w00-1;k>=y0-1;k--)
        {  /* long division */

            carry=0;
            if (w0->w[k+1]==ldy) /* guess next quotient digit */
            {
                attemp=(mr_small)(-1);
                ra=ldy+w0->w[k];
                if (ra<ldy) carry=1;
            }
            else
            {
                MR_BOT(dble)=w0->w[k];
                MR_TOP(dble)=w0->w[k+1];
                attemp=(mr_small)(dble/ldy);
                ra=(mr_small)(dble-(mr_large)attemp*ldy);
            }

            while (carry==0)
            {
                dble=(mr_large)attemp*sdy;
                r=(mr_small)dble;
                tst=MR_TOP(dble);

                if (tst< ra || (tst==ra && r<=w0->w[k-1])) break;
                attemp--;  /* refine guess */
                ra+=ldy;
                if (ra<ldy) carry=1;
            }
            m=k-y0+1;
            if (attemp>0)
            { /* do partial subtraction */
                borrow=0;

                for (i=0;i<y0;i++)
                {
					dble=(mr_large)attemp*y->w[i]+borrow;
					dig=(mr_small)dble;
					borrow=MR_TOP(dble);
                  if (w0->w[m+i]<dig) borrow++;
                  w0->w[m+i]-=dig;
                }

                if (w0->w[k+1]<borrow)
                {  /* whoops! - over did it */
                    w0->w[k+1]=0;
                    carry=0;
                    for (i=0;i<y0;i++)
                    {  /* compensate for error ... */
                        psum=w0->w[m+i]+y->w[i]+carry;
                        if (psum>y->w[i]) carry=0;
                        if (psum<y->w[i]) carry=1;
                        w0->w[m+i]=psum;
                    }
                    attemp--;  /* ... and adjust guess */
                }
                else w0->w[k+1]-=borrow;
            }
            if (k==w00-1 && attemp==0) w00--;
            else if (y!=z) z->w[m]=attemp;
        }
    }
    else
    {   /* have to do it the hard way */
        if (d!=1) mr_pmul( w0,d,w0);
        ldy=y->w[y0-1];
        sdy=y->w[y0-2];

        for (k=w00-1;k>=y0-1;k--)
        {  /* long division */


            if (w0->w[k+1]==ldy) /* guess next quotient digit */
            {
                attemp=mr_mip->base-1;
                ra=ldy+w0->w[k];
            }
            else 
            {
                dble=(mr_large)w0->w[k+1]*mr_mip->base+w0->w[k];
                attemp=(mr_small)MR_LROUND(dble/ldy);
                ra=(mr_small)(dble-(mr_large)attemp*ldy);
            }

            while (ra<mr_mip->base)
            {
                dble=(mr_large)sdy*attemp;
                tst=(mr_small)MR_LROUND(dble/mr_mip->base);
                r=(mr_small)(dble-(mr_large)tst*mr_mip->base);

                if (tst< ra || (tst==ra && r<=w0->w[k-1])) break;
                attemp--;  /* refine guess */
                ra+=ldy;
            }    
            m=k-y0+1;
            if (attemp>0)
            { /* do partial subtraction */
                borrow=0;
                for (i=0;i<y0;i++)
                {
                  dble=(mr_large)attemp*y->w[i]+borrow;
                  borrow=(mr_small)MR_LROUND(dble/mr_mip->base);
                  dig=(mr_small)(dble-(mr_large)borrow*mr_mip->base);

                  if (w0->w[m+i]<dig)
                  { /* set borrow */
                      borrow++;
                      w0->w[m+i]+=(mr_mip->base-dig);
                  }
                  else w0->w[m+i]-=dig;
                }
                if (w0->w[k+1]<borrow)
                {  /* whoops! - over did it */
                    w0->w[k+1]=0;
                    carry=0;
                    for (i=0;i<y0;i++)
                    {  /* compensate for error ... */
                        psum=w0->w[m+i]+y->w[i]+carry;
                        carry=0;
                        if (psum>=mr_mip->base)
                        {
                            carry=1;
                            psum-=mr_mip->base;
                        }
                        w0->w[m+i]=psum;
                    }
                    attemp--;  /* ... and adjust guess */
                }
                else
                    w0->w[k+1]-=borrow;
            }
            if (k==w00-1 && attemp==0) w00--;
            else if (y!=z) z->w[m]=attemp;
        }
    }
    if (y!=z) z->len=((w00-y0+1)|sz); /* set sign and length of result */

    w0->len=y0;

    mr_lzero(y);
    mr_lzero(z);

    if (x!=z)
    {
        mr_lzero(w0);

        if (d!=1) mr_sdiv( w0,d,x);

        else copy(w0,x);
        if (x->len!=0) x->len|=sx;
    }

    if (d!=1) mr_sdiv( y,d,y);

    y->len|=sy;
    mr_mip->check=check;

}


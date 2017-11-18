/*
 *   Extended Greatest Common Divisor module.
 *   mrxgcd.c
 */

#include <stdio.h>
#include "miracl.h"


/* greatest common divisor by Euclids method  *
   * extended to also calculate xd and yd where *
   *      z = x.xd + y.yd = gcd(x,y)            *
   * if xd, yd not distinct, only xd calculated *
   * z only returned if distinct from xd and yd *
   * xd will always be positive, yd negative    */
int xgcd(big x,big y,big xd,big yd,big z)
{ 
	int s,n,iter;
    mr_small r,a,b,c,d;
    mr_small q,m,sr;
    mr_small u,v,temp,lr;

    BOOL last,dplus=TRUE;
    big t;


/*	Pass params into function	*/
    copy(x,mr_mip->w1);
    copy(y,mr_mip->w2);
/*    s=exsign(mr_mip->w1);	*/
	s=MINUS;
	if ((mr_mip->w1->len&(MR_MSBIT))==0) s=PLUS;
/*    insign(PLUS,mr_mip->w1);	*/
	if (mr_mip->w1->len!=0)
		mr_mip->w1->len&=MR_OBITS;
/*    insign(PLUS,mr_mip->w2);	*/
	if (mr_mip->w2->len!=0)
		mr_mip->w2->len&=MR_OBITS;
    convert( 1,mr_mip->w3);
    zero(mr_mip->w4);
    last=FALSE;
    a=b=c=d=0;
    iter=0;

    while (size(mr_mip->w2)!=0)
    {
        if (b==0)
        { /* update mr_mip->w1 and mr_mip->w2 */
            divide( mr_mip->w1,mr_mip->w2,mr_mip->w5);
            t=mr_mip->w1,mr_mip->w1=mr_mip->w2,mr_mip->w2=t;    /* swap(mr_mip->w1,mr_mip->w2) */
            multiply( mr_mip->w4,mr_mip->w5,mr_mip->w0);
			myselect( mr_mip->w3,PLUS,mr_mip->w0,mr_mip->w3);
            t=mr_mip->w3,mr_mip->w3=mr_mip->w4,mr_mip->w4=t;    /* swap(xd,yd) */
            iter++;
        }
        else
        {

/*  printf("a= %d b= %d c= %d  d= %d \n",a,b,c,d);   */

            mr_pmul( mr_mip->w1,c,mr_mip->w5);   /* c*w1 */
            mr_pmul( mr_mip->w1,a,mr_mip->w1);   /* a*w1 */
            mr_pmul( mr_mip->w2,b,mr_mip->w0);   /* b*w2 */
            mr_pmul( mr_mip->w2,d,mr_mip->w2);   /* d*w2 */

            if (!dplus)
            {
                mr_psub( mr_mip->w0,mr_mip->w1,mr_mip->w1); /* b*w2-a*w1 */
                mr_psub( mr_mip->w5,mr_mip->w2,mr_mip->w2); /* c*w1-d*w2 */
            }
            else
            {
                mr_psub( mr_mip->w1,mr_mip->w0,mr_mip->w1); /* a*w1-b*w2 */
                mr_psub( mr_mip->w2,mr_mip->w5,mr_mip->w2); /* d*w2-c*w1 */
            }
            mr_pmul( mr_mip->w3,c,mr_mip->w5);
            mr_pmul( mr_mip->w3,a,mr_mip->w3);
            mr_pmul( mr_mip->w4,b,mr_mip->w0);
            mr_pmul( mr_mip->w4,d,mr_mip->w4);
    
            if (a==0) copy(mr_mip->w0,mr_mip->w3);
            else      mr_padd( mr_mip->w3,mr_mip->w0,mr_mip->w3);
            mr_padd( mr_mip->w4,mr_mip->w5,mr_mip->w4);
        }
        if (size(mr_mip->w2)==0) break;

        n=(int)mr_mip->w1->len;
        if (n==1)
        {
            last=TRUE;
            u=mr_mip->w1->w[0];
            v=mr_mip->w2->w[0];
        }
        else
        {
            m=mr_mip->w1->w[n-1]+1;
            if (mr_mip->base==0)
            {
                if (m==0)
                {
                    u=mr_mip->w1->w[n-1];
                    v=mr_mip->w2->w[n-1];   
                }
                else
                {
                    u=muldvm(mr_mip->w1->w[n-1],mr_mip->w1->w[n-2],m,&sr);
                    v=muldvm(mr_mip->w2->w[n-1],mr_mip->w2->w[n-2],m,&sr);
                }
            }
            else
            {
                u=muldiv(mr_mip->w1->w[n-1],mr_mip->base,mr_mip->w1->w[n-2],m,&sr);
                v=muldiv(mr_mip->w2->w[n-1],mr_mip->base,mr_mip->w2->w[n-2],m,&sr);
            }
        }

        dplus=TRUE;
        a=1; b=0; c=0; d=1;

        forever
        { /* work only with most significant piece */
            if (last)
            {
                if (v==0) break;
				if (u<2*v) q=1;
				if (u<3*v) q=2;
				q=MR_DIV(u,v);

                if (q==0) break;
            }
            else
            {
                if (dplus)
                { 
                    if (v-c==0 || v+d==0) break;
					if ((u+a)<2*(v-c)) q=1;
					if ((u+a)<3*(v-c)) q=2;
					q=MR_DIV(u+a,v-c);

                    if (q==0) break;

					if ((u-b)<2*(v+d)) temp=1;
					if ((u-b)<3*(v+d)) temp=2;
					temp=MR_DIV(u-b,v+d);
					if(q!=temp) break;
                }
                else 
                {
                    if (v+c==0 || v-d==0) break;
					if ((u-a)<2*(v+c)) q=1;
					if ((u-a)<3*(v+c)) q=2;
					q=MR_DIV(u-a,v+c);
                    if (q==0) break;
					if ((u+b)<2*(v-d)) temp=1;
					if ((u+b)<3*(v-d)) temp=2;
					temp=MR_DIV(u+b,v-d);
					if(q!=temp) break;
                }
            }

            if (q==1)
            {
                if (b+d >= MAXBASE) break; 
                r=a+c;  a=c; c=r;
                r=b+d;  b=d; d=r;
                lr=u-v; u=v; v=lr;      
            }
            else
            { 
                if (q>=MR_DIV(MAXBASE-b,d)) break;
                r=a+q*c;  a=c; c=r;
                r=b+q*d;  b=d; d=r;
                lr=u-q*v; u=v; v=lr;
            }
            iter++;
            dplus=!dplus;
        }
        iter%=2;
    }

    if (s==MINUS) iter++;
    if (iter%2==1) 
		myselect( y,MINUS,mr_mip->w3,mr_mip->w3);

    if (xd!=yd)
    {
		copy(x,mr_mip->w2);
		if (x,mr_mip->w2->len!=0) x,mr_mip->w2->len^=MR_MSBIT;
/* Multiply, Add and Divide; q=(x*y+z)/w remainder r   *
   * returns remainder only if w=q, quotient only if q=r *
   * add done only if x, y and z are distinct.           */
		mr_mip->check=OFF;           /* turn off some error checks */
		if (y==mr_mip->w4)		
		{
			return 0;
		}
		multiply( mr_mip->w2,mr_mip->w3,mr_mip->w0); 
		myselect( mr_mip->w0,PLUS,mr_mip->w1,mr_mip->w0);
		divide( mr_mip->w0,y,mr_mip->w4);
		if (q!=mr_mip->w4) copy(mr_mip->w0,mr_mip->w4);
		mr_mip->check=ON;

        copy(mr_mip->w4,yd);
    }
    copy(mr_mip->w3,xd);
    if (z!=xd && z!=yd) copy(mr_mip->w1,z);

    return (size(mr_mip->w1));
}

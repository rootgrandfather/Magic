/*
 *   Montgomery's method for modular arithmetic without division.
 *   mrmonty.c 
 */

#include <stdio.h>
#include "miracl.h"

#define mr_abs(x)  ((x)<0? (-(x)) : (x))


void nres( big x,big y)
{ /* convert x to n-residue format */

    if (mr_mip->modulus==NULL)			
        return;
    copy(x,y);
    divide( y,mr_mip->modulus,mr_mip->modulus);
    if (size(y)<0)
		myselect( y,PLUS,mr_mip->modulus,y);

    mr_mip->check=OFF;

    mr_shift( y,(int)mr_mip->modulus->len,mr_mip->w0);
    divide( mr_mip->w0,mr_mip->modulus,mr_mip->modulus);
    mr_mip->check=ON;
    copy(mr_mip->w0,y);
}

void redc( big x,big y)
{ /* Montgomery's REDC function p. 520 */
  /* also used to convert n-residues back to normal form */
    mr_small carry,delay_carry,m,ndash,*w0g,*mg;
    mr_large dble;
    int i,j,rn,rn2;
    big w0,modulus;

    w0=mr_mip->w0;        /* get these into local variables (for inline assembly) */
    modulus=mr_mip->modulus;
    ndash=mr_mip->ndash;

    copy(x,w0);

    delay_carry=0;
    rn=(int)modulus->len;
    rn2=rn+rn;
    if (mr_mip->base==0) 
    {
      mg=modulus->w;
      w0g=w0->w;
      for (i=0;i<rn;i++)
      {
       /* inline - substitutes for loop below */
        muldvd(w0->w[i],ndash,0,&m);   /* Note that after this time   */
        carry=0;                       /* around the loop, w0[i]=0    */

        for (j=0;j<rn;j++)
        {
            dble=(mr_large)m*modulus->w[j]+carry+w0->w[i+j];
            w0->w[i+j]=(mr_small)dble;
            carry=MR_TOP(dble);
        }
        w0->w[rn+i]+=delay_carry;
        if (w0->w[rn+i]<delay_carry) delay_carry=1;
        else delay_carry=0;
        w0->w[rn+i]+=carry;
        if (w0->w[rn+i]<carry) delay_carry=1; 
      }
	}
    else for (i=0;i<rn;i++) 
    {

        muldiv(w0->w[i],ndash,0,mr_mip->base,&m);

        carry=0;
        for (j=0;j<rn;j++)
        {
          dble=(mr_large)m*modulus->w[j]+carry+w0->w[i+j];
          carry=(mr_small)MR_LROUND(dble/mr_mip->base);
          w0->w[i+j]=(mr_small)(dble-(mr_large)carry*mr_mip->base);  

        }
        w0->w[rn+i]+=(delay_carry+carry);
        delay_carry=0;
        if (w0->w[rn+i]>=mr_mip->base)
        {
            w0->w[rn+i]-=mr_mip->base;
            delay_carry=1; 
        }
    }
    w0->w[rn2]=delay_carry;
    w0->len=rn2+1;
    mr_shift( w0,(-rn),w0);
    mr_lzero(w0);
    
    if (compare(w0,modulus)>=0) mr_psub( w0,modulus,y);
    else copy(w0,y);

/*	deal with point	*/
	w0=NULL;
	modulus=NULL;
	w0g=NULL;
	mg=NULL;
}

void nres_negate( big x, big w)
{
    if (size(x)==0) zero(w);
    else mr_psub( mr_mip->modulus,x,w);    
}

void nres_modadd( big x,big y,big w)
{ /* modular addition */
    mr_padd( x,y,w);
    if (compare(w,mr_mip->modulus)>=0) mr_psub( w,mr_mip->modulus,w);
}

void nres_modsub( big x,big y,big w)
{ /* modular subtraction */
    if (compare(x,y)>=0)
            mr_psub( x,y,w);
    else
    {
            mr_psub( y,x,w);
            mr_psub( mr_mip->modulus,w,w);
    }
}

int nres_moddiv( big x,big y,big w)
{ /* Modular division using n-residues w=x/y mod n */
    int gcd;

    if (x==y)			
    { /* Illegal parameter usage */
        return 0;
    }
    redc( y,mr_mip->w6);
    gcd=xgcd( mr_mip->w6,mr_mip->modulus,mr_mip->w6,mr_mip->w6,mr_mip->w6);
/* Multiply, Add and Divide; q=(x*y+z)/w remainder r   *
   * returns remainder only if w=q, quotient only if q=r *
   * add done only if x, y and z are distinct.           */
    mr_mip->check=OFF;           /* turn off some error checks */
    if (mr_mip->modulus==w)			
        return 0;
    multiply( x,mr_mip->w6,mr_mip->w0);
    divide( mr_mip->w0,mr_mip->modulus,mr_mip->modulus);
    if (mr_mip->modulus!=w) copy(mr_mip->w0,w);
    mr_mip->check=ON;
    return gcd;
}

void nres_modmult( big x,big y,big w)
{ /* Modular multiplication using n-residues w=x*y mod n */
    mr_mip->check=OFF;
    multiply( x,y,mr_mip->w0);
    redc( mr_mip->w0,w);
    mr_mip->check=ON;
}

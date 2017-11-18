/*
 *   Methods for modular exponentiation
 *   mrpower.c 
 */

#include <stdio.h>
#include "miracl.h"


void nres_powltr( int x,big y,big w)
{ /* calculates w=x^y mod z using Left to Right Method   */
  /* uses only n^2 modular squarings, because x is small */
  /* Note: x is NOT an nresidue */
    int i,nb;

    copy(y,mr_mip->w1);

    zero(w);
    if (x==0) 
    {
        if (size(mr_mip->w1)==0) 
        { /* 0^0 = 1 */
            convert( 1,w);
            nres( w,w);
        }
        return;
    }

    convert( 1,w);
    nres( w,w);
    if (size(mr_mip->w1)==0)    return;

    if (size(mr_mip->w1)<0)	
	{
		return;
	}

    if (mr_mip->base==mr_mip->base2)
    { 
        nb=logb2( mr_mip->w1);
        convert( x,w);
        nres( w,w);
        if (nb>1) for (i=nb-2;i>=0;i--)
        { /* Left to Right binary method */

            nres_modmult( w,w,w);
			if ((mr_mip->w1->w[i/mr_mip->lg2b] & ((mr_small)1<<(i%mr_mip->lg2b))) >0)
            { /* this is quick */
                premult( w,x,w);
                divide( w,mr_mip->modulus,mr_mip->modulus);
            }
        }
    }    
    else
    {
        expint( 2,logb2( mr_mip->w1)-1,mr_mip->w2);
        while (size(mr_mip->w2)!=0)
        { /* Left to Right binary method */

            nres_modmult( w,w,w);
            if (compare(mr_mip->w1,mr_mip->w2)>=0)
            {
                premult( w,x,w);
                divide( w,mr_mip->modulus,mr_mip->modulus);
				myselect( mr_mip->w1,MINUS,mr_mip->w2,mr_mip->w1);
            }
            subdiv( mr_mip->w2,2,mr_mip->w2);
        }
    }

}

void nres_powmod( big x,big y,big w)
{  /*  calculates w=x^y mod z, using m-residues       *
    *  See "Analysis of Sliding Window Techniques for *
    *  Exponentiation, C.K. Koc, Computers. Math. &   *
    *  Applic. Vol. 30 pp17-24 1995. Uses work-space  *
    *  variables for pre-computed table. */
    int i,j,k,t,nb,nbw,nzs,n;
    big table[16];

    copy(y,mr_mip->w1);
    copy(x,mr_mip->w3);

    zero(w);
    if (size(x)==0)
    {
       if (size(mr_mip->w1)==0)
       { /* 0^0 = 1 */
           convert( 1,w);
           nres( w,w);
       } 
       return;
    }

    convert( 1,w);
    nres( w,w);
    if (size(mr_mip->w1)==0) return;

    if (size(mr_mip->w1)<0)	
	{
		return;
	}

    if (mr_mip->base==mr_mip->base2)
    { /* build a table. Up to 5-bit sliding windows. Windows with
       * two adjacent 0 bits simply won't happen */
        table[0]=mr_mip->w3; table[1]=mr_mip->w4; table[2]=mr_mip->w5; table[3]=mr_mip->w16;
        table[4]=NULL;  table[5]=mr_mip->w6; table[6]=mr_mip->w17; table[7]=mr_mip->w8;
        table[8]=NULL;  table[9]=NULL;  table[10]=mr_mip->w9; table[11]=mr_mip->w10;
        table[12]=NULL; table[13]=mr_mip->w11; table[14]=mr_mip->w12; table[15]=mr_mip->w13;

        nres_modmult( mr_mip->w3,mr_mip->w3,mr_mip->w2);  /* x^2 */
        n=15;
        j=0;
        do
        { /* pre-computations */
            t=1; k=j+1;
            while (table[k]==NULL) {k++; t++;}
            copy(table[j],table[k]);
            for (i=0;i<t;i++) nres_modmult( table[k],mr_mip->w2,table[k]);
            j=k;
        } while (j<n);

        nb=logb2( mr_mip->w1);
        copy(mr_mip->w3,w);
        if (nb>1) for (i=nb-2;i>=0;)
        { /* Left to Right method */

            n=mr_window( mr_mip->w1,i,&nbw,&nzs); 
            for (j=0;j<nbw;j++)
                    nres_modmult( w,w,w);
            if (n>0) nres_modmult( w,table[n/2],w); 
            i-=nbw;
            if (nzs)
            {
                for (j=0;j<nzs;j++) nres_modmult( w,w,w);
                i-=nzs;
            }
        }
    }
    else
    {
        copy(mr_mip->w3,mr_mip->w2);
        forever
        { /* "Russian peasant" Right-to-Left exponentiation */

            if (subdiv( mr_mip->w1,2,mr_mip->w1)!=0)
                nres_modmult( w,mr_mip->w2,w);
            if (size(mr_mip->w1)==0) break;
            nres_modmult( mr_mip->w2,mr_mip->w2,mr_mip->w2);
        }
    }
}

BOOL nres_sqroot( big x,big w)
{ /* w=sqrt(x) mod p. This depends on p being prime! */
    int i,n,e,r,cat;
    BOOL pp;

    copy(x,mr_mip->w15);
    zero(w);
    if (size(mr_mip->w15)==0)
    {
        zero(w);
        return TRUE;
    }

    convert( 1,w);
    nres( w,w);
    if (compare(w,mr_mip->w15)==0)
    {
        return TRUE;
    }

    cat=remain( mr_mip->modulus,8);
    switch(cat)
    {
    case 0: case 2: case 4: case 6: 
        zero(w);
        return FALSE;

    case 3: case 7:                          /* easy case */
		convert( 1,mr_mip->w0);
		myselect( mr_mip->modulus,PLUS,mr_mip->w0,mr_mip->w14);

        subdiv( mr_mip->w14,4,mr_mip->w14);
        nres_powmod( x,mr_mip->w14,w);
        nres_modmult( w,w,mr_mip->w14);
        if (compare(mr_mip->w14,mr_mip->w15)==0) 
            return TRUE;
        zero(w);
        return FALSE;

    case 5:
        nres_modadd( mr_mip->w15,mr_mip->w15,mr_mip->w15); /* 2x */
		convert( 5,mr_mip->w0);
		myselect( mr_mip->modulus,MINUS,mr_mip->w0,mr_mip->w14);

        subdiv( mr_mip->w14,8,mr_mip->w14);
        nres_powmod( mr_mip->w15,mr_mip->w14,w);
        nres_modmult( w,w,mr_mip->w14);
        nres_modmult( mr_mip->w15,mr_mip->w14,mr_mip->w14);
        convert( 1,mr_mip->w1);
        nres( mr_mip->w1,mr_mip->w1);
        nres_modsub( mr_mip->w14,mr_mip->w1,mr_mip->w14);
        nres_modmult( mr_mip->w14,w,w);
        if (remain( mr_mip->w15,2)!=0)
			  myselect( mr_mip->w15,PLUS,mr_mip->modulus,mr_mip->w15);
        subdiv( mr_mip->w15,2,mr_mip->w15);                /* x */
        nres_modmult( w,mr_mip->w15,w);
        nres_modmult( w,w,mr_mip->w14);
        if (compare(mr_mip->w14,mr_mip->w15)==0) 
            return TRUE;
        zero(w);
        return FALSE;
        
    case 1:                      /* difficult case. Shank's method */
		convert( 1,mr_mip->w0);
		myselect( mr_mip->modulus,MINUS,mr_mip->w0,mr_mip->w14);

        e=0;
		while (remain( mr_mip->w14,2)==0)
        {
            subdiv( mr_mip->w14,2,mr_mip->w14);
            e++;
        }
        for (r=2;;r++)
        {
            convert( 1,mr_mip->w3);
            nres( mr_mip->w3,mr_mip->w3);             /* w3=1 */
            nres_powltr( r,mr_mip->w14,w);
            if (compare(w,mr_mip->w3)==0) continue;
            copy(w,mr_mip->w4);
            nres_negate( mr_mip->w3,mr_mip->w1);      /* w1 = -1 */
            pp=FALSE;
            for (i=1;i<e;i++)         
            { /* check for composite modulus */

                if (compare(mr_mip->w4,mr_mip->w1)==0) pp=TRUE;
                nres_modmult( mr_mip->w4,mr_mip->w4,mr_mip->w4);
                if (!pp && compare(mr_mip->w4,mr_mip->w3)==0)
                {              
                    zero(w);
                    return FALSE;
                }
            }   
            if (compare(mr_mip->w4,mr_mip->w1)==0) break;
            if (!pp)
            {
                zero(w);
                return FALSE;
            }
        }                                                /* w= y    */
        copy(mr_mip->w15,mr_mip->w3);                    /* w3 = x  */
        nres_powmod( mr_mip->w3,mr_mip->w14,mr_mip->w15);  /* w15 = w */
		convert( 1,mr_mip->w0);
		myselect( mr_mip->w14,PLUS,mr_mip->w0,mr_mip->w14);

        subdiv( mr_mip->w14,2,mr_mip->w14);
        nres_powmod( mr_mip->w3,mr_mip->w14,mr_mip->w14);  /* w14 = v */
        forever
        {
            convert( 1,mr_mip->w1);
            nres( mr_mip->w1,mr_mip->w1);
            if (compare(w,mr_mip->w1)==0)  break;
            copy(mr_mip->w15,mr_mip->w2);
            for (n=0;compare(mr_mip->w2,mr_mip->w1)!=0;n++)
                nres_modmult( mr_mip->w2,mr_mip->w2,mr_mip->w2);
            if (n>=e)
            {
                zero(w);
                return FALSE;
            }
            r=e-n-1;
            for (i=0;i<r;i++) nres_modmult( w,w,w);
            nres_modmult( mr_mip->w14,w,mr_mip->w14);
            nres_modmult( w,w,w);
            nres_modmult( mr_mip->w15,w,mr_mip->w15);
            e=n;
        }
        copy(mr_mip->w14,w);
        nres_modmult( w,w,mr_mip->w14);
        if (compare(mr_mip->w14,mr_mip->w3)==0) 
            return TRUE;
        zero(w);
        return FALSE;      
    }
    return FALSE;
}

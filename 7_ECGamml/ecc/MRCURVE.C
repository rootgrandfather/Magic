/*
 *   Elliptic curve routines
 *   mrcurve.c
 *
 *   Assumes Weierstrass equation y^2 = x^3 + Ax + B
 *   See IEEE P1363 Draft Standard 
 *
 *   Uses Montgomery's representation internally
 */

#include <stdlib.h>
#include <stdio.h>
#include "miracl.h"


#define mr_abs(x)  ((x)<0? (-(x)) : (x))

/* initialise elliptic curve */

void ecurve_init( big a,big b,big p)
{ /* Initialize the active ecurve    *
   * Asize indicate size of A        *
   * Bsize indicate size of B        */
    int as;


/* prepare Montgomery modulus */ 
	if((!mr_mip->modulus)||compare(p,mr_mip->modulus))
	{
		if (size(p)<=2) 			
			return ;
		zero(mr_mip->w6);
		zero(mr_mip->w16);

		mr_mip->w6->len=2;
		mr_mip->w6->w[0]=0;
		mr_mip->w6->w[1]=1;    /* w6 = base */
		mr_mip->w16->len=1;
		mr_mip->w16->w[0]=p->w[0];  /* w16 = n mod base */
		if (xgcd( mr_mip->w16,mr_mip->w6,mr_mip->w14,mr_mip->w14,mr_mip->w14)!=1)
			return ;
		mr_mip->ndash=mr_mip->base-mr_mip->w14->w[0]; /* = N' mod b */
		if (mr_mip->modulus==NULL) mr_mip->modulus=mirvar( 0);
		copy(p,mr_mip->modulus);
	}

    mr_mip->Asize=size(a);
    if (mr_abs(mr_mip->Asize)==MR_TOOBIG)
    {
        if (mr_mip->Asize>=0)
        { /* big positive number - check it isn't minus something small */
           copy(a,mr_mip->w1);
           divide( mr_mip->w1,p,p);
		   myselect( p,MINUS,mr_mip->w1,mr_mip->w1);
           as=size(mr_mip->w1);
           if (as<MR_TOOBIG) mr_mip->Asize=-as;
           else
           {
               if (mr_mip->A==NULL) mr_mip->A=mirvar( 0);
               nres( a,mr_mip->A);
           }
        }
        else
        {
               if (mr_mip->A==NULL) mr_mip->A=mirvar( 0);
               nres( a,mr_mip->A);
        }
    }
    mr_mip->Bsize=size(b);
    if (mr_abs(mr_mip->Bsize)==MR_TOOBIG)
    {
        if (mr_mip->B==NULL) mr_mip->B=mirvar( 0);
        nres( b,mr_mip->B);

    }
    return;
}

epoint* epoint_init()
{ /* initialise epoint to point at infinity. */
    epoint *p;
    char *ptr;


/* Create space for whole structure in one heap access */ 

	p=(epoint *)calloc(sizeof(epoint)+3*mr_mip->size,1);
	if (p==NULL) 
		return NULL;
        
    ptr=(char *)p+sizeof(epoint);
    p->X=mirvar_mem( ptr,0);
    p->Y=mirvar_mem( ptr,1);
	p->Z=mirvar_mem( ptr,2);

    p->marker=MR_EPOINT_INFINITY;


    return p;
}

void epoint_free(epoint *p)
{ /* clean up point */
    zero(p->X);
    zero(p->Y);
    if (p->marker==MR_EPOINT_GENERAL) zero(p->Z);
    if (p!=NULL) 
		free(p);
}        

int epoint_get( epoint* p,big x,big y)
{ /* Get point co-ordinates in affine, normal form       *
   * (converted from projective, Montgomery form)        *
   * if x==y, supplies x only. Return value is Least     *
   * Significant Bit of y (useful for point compression) */

    int lsb;

    if (p->marker==MR_EPOINT_INFINITY)
    {
        zero(x);
        zero(y);
        return 0;
    }

    if (!epoint_norm( p)) 
    { /* not possible ! */
        return (-1);
    }

    redc( p->X,x);
    redc( p->Y,mr_mip->w1);

    if (x!=y) copy(mr_mip->w1,y);
    lsb=remain( mr_mip->w1,2); 
    return lsb;
}

BOOL epoint_norm( epoint *p)
{ /* normalise a point */
    
    if (p->marker!=MR_EPOINT_GENERAL) return TRUE;

    convert( 1,mr_mip->w8);
    nres( mr_mip->w8,mr_mip->w8);  

    if (nres_moddiv( mr_mip->w8,p->Z,mr_mip->w8)>1) /* 1/Z  */
    {
        return FALSE;
    }
    
    nres_modmult( mr_mip->w8,mr_mip->w8,mr_mip->w1);/* 1/ZZ */
    nres_modmult( p->X,mr_mip->w1,p->X);            /* X/ZZ */
    nres_modmult( mr_mip->w1,mr_mip->w8,mr_mip->w1);/* 1/ZZZ */
    nres_modmult( p->Y,mr_mip->w1,p->Y);            /* Y/ZZZ */

    convert( 1,mr_mip->w8);
    nres( mr_mip->w8,p->Z);
   
    p->marker=MR_EPOINT_NORMALIZED;
    return TRUE;
}

static void ecurve_double( epoint *p)
{ /* double epoint on active ecurve */

    if (p->marker==MR_EPOINT_INFINITY) 
    { /* 2 times infinity == infinity ! */
        return;
    }

    if (size(p->Y)==0)
    { /* set to point at infinity */
        convert( 1,mr_mip->w1);
        nres( mr_mip->w1,p->X);
        nres( mr_mip->w1,p->Y);
        p->marker=MR_EPOINT_INFINITY;
        return;
    }
    convert( 1,mr_mip->w1);
    if (p->marker==MR_EPOINT_NORMALIZED) nres( mr_mip->w1,mr_mip->w2);
    else nres_modmult( p->Z,p->Z,mr_mip->w2);
    if (mr_abs(mr_mip->Asize) < MR_TOOBIG)
    {
        if (mr_mip->Asize==(-3))
        { /* a is -3. Goody. */
            nres_modsub( p->X,mr_mip->w2,mr_mip->w3);
            nres_modadd( p->X,mr_mip->w2,mr_mip->w8);
            nres_modmult( mr_mip->w3,mr_mip->w8,mr_mip->w3);
            nres_modadd( mr_mip->w3,mr_mip->w3,mr_mip->w8);
            nres_modadd( mr_mip->w8,mr_mip->w3,mr_mip->w8);
        }
        else
        { /* a is small */
            if (mr_mip->Asize!=0)
            { /* a is non zero! */
                nres_modmult( mr_mip->w2,mr_mip->w2,mr_mip->w3);
			    premult( mr_mip->w3,mr_mip->Asize,mr_mip->w3);
				divide( mr_mip->w3,mr_mip->modulus,mr_mip->modulus);
				if (size(mr_mip->w3)<0) 
				myselect( mr_mip->w3,PLUS,mr_mip->modulus,mr_mip->w3);
            }
            nres_modmult( p->X,p->X,mr_mip->w1);
            nres_modadd( mr_mip->w1,mr_mip->w1,mr_mip->w8);
            nres_modadd( mr_mip->w8,mr_mip->w1,mr_mip->w8);
            if (mr_mip->Asize!=0) nres_modadd( mr_mip->w8,mr_mip->w3,mr_mip->w8);
        }
    }
    else
    { /* a is not special */
        nres_modmult( mr_mip->w2,mr_mip->w2,mr_mip->w3);
        nres_modmult( mr_mip->w3,mr_mip->A,mr_mip->w3);
        nres_modmult( p->X,p->X,mr_mip->w1);
        nres_modadd( mr_mip->w1,mr_mip->w1,mr_mip->w8);
        nres_modadd( mr_mip->w8,mr_mip->w1,mr_mip->w8);
        nres_modadd( mr_mip->w8,mr_mip->w3,mr_mip->w8);        
    }

/* w8 contains numerator of slope 3x^2+A.z^4  *
 * denominator is now placed in Z             */
    if (p->marker==MR_EPOINT_NORMALIZED)
        copy(p->Y,p->Z);
    else nres_modmult( p->Y,p->Z,p->Z);

    nres_modadd( p->Z,p->Z,p->Z);
    nres_modmult( p->Y,p->Y,mr_mip->w2);
    nres_modmult( p->X,mr_mip->w2,mr_mip->w3);
    nres_modadd( mr_mip->w3,mr_mip->w3,mr_mip->w3);
    nres_modadd( mr_mip->w3,mr_mip->w3,mr_mip->w3);
    nres_modmult( mr_mip->w8,mr_mip->w8,mr_mip->w1);
    nres_modadd( mr_mip->w3,mr_mip->w3,p->X);
    nres_modsub( mr_mip->w1,p->X,p->X);
    nres_modmult( mr_mip->w2,mr_mip->w2,mr_mip->w2);
    nres_modadd( mr_mip->w2,mr_mip->w2,mr_mip->w2);
    nres_modadd( mr_mip->w2,mr_mip->w2,mr_mip->w2);
    nres_modadd( mr_mip->w2,mr_mip->w2,mr_mip->w2);
    nres_modsub( mr_mip->w3,p->X,mr_mip->w3);
    nres_modmult( mr_mip->w3,mr_mip->w8,mr_mip->w3);
    nres_modsub( mr_mip->w3,mr_mip->w2,p->Y);
  
    p->marker=MR_EPOINT_GENERAL;
    return;
}
   
static BOOL ecurve_padd( epoint *p,epoint *pa)
{ /* primitive add two epoints on the active ecurve - pa+=p;   *
   * note that if p is normalized, its Z coordinate isn't used */
 
    if (p->marker!=MR_EPOINT_NORMALIZED)    
    {
        nres_modmult( p->Z,p->Z,mr_mip->w6);
        nres_modmult( pa->X,mr_mip->w6,mr_mip->w1);
        nres_modmult( mr_mip->w6,p->Z,mr_mip->w6);
        nres_modmult( pa->Y,mr_mip->w6,mr_mip->w8);
    }
    else
    {
        copy(pa->X,mr_mip->w1);
        copy(pa->Y,mr_mip->w8);
    }
    if (pa->marker==MR_EPOINT_NORMALIZED)
    {
        convert( 1,mr_mip->w6);
        nres( mr_mip->w6,mr_mip->w6);
    }
    else nres_modmult( pa->Z,pa->Z,mr_mip->w6);
    nres_modmult( p->X,mr_mip->w6,mr_mip->w4);
    if (pa->marker!=MR_EPOINT_NORMALIZED) 
        nres_modmult( mr_mip->w6,pa->Z,mr_mip->w6);
    nres_modmult( p->Y,mr_mip->w6,mr_mip->w5);
    nres_modsub( mr_mip->w1,mr_mip->w4,mr_mip->w1);
    nres_modsub( mr_mip->w8,mr_mip->w5,mr_mip->w8);

/* w8 contains the numerator of the slope */

    if (size(mr_mip->w1)==0)
    {
        if (size(mr_mip->w8)==0)
        { /* should have doubled ! */
           return FALSE; 
        }
        else
        { /* point at infinity */
			convert( 1,mr_mip->w1);
			nres( mr_mip->w1,pa->X);
			nres( mr_mip->w1,pa->Y);
			pa->marker=MR_EPOINT_INFINITY;
            return TRUE;
        }
    }
    nres_modadd( mr_mip->w4,mr_mip->w4,mr_mip->w6);
    nres_modadd( mr_mip->w1,mr_mip->w6,mr_mip->w4);
    nres_modadd( mr_mip->w5,mr_mip->w5,mr_mip->w6);
    nres_modadd( mr_mip->w8,mr_mip->w6,mr_mip->w5);
    
    if (p->marker!=MR_EPOINT_NORMALIZED)
    { 
        if (pa->marker!=MR_EPOINT_NORMALIZED) 
            nres_modmult( pa->Z,p->Z,mr_mip->w3);
        else
            copy(p->Z,mr_mip->w3);
        nres_modmult( mr_mip->w3,mr_mip->w1,pa->Z);
    }
    else
    {
        if (pa->marker!=MR_EPOINT_NORMALIZED)
            nres_modmult( pa->Z,mr_mip->w1,pa->Z);
        else
            copy(mr_mip->w1,pa->Z);
    }
    nres_modmult( mr_mip->w1,mr_mip->w1,mr_mip->w6);
    nres_modmult( mr_mip->w1,mr_mip->w6,mr_mip->w1);
    nres_modmult( mr_mip->w6,mr_mip->w4,mr_mip->w6);
    nres_modmult( mr_mip->w8,mr_mip->w8,mr_mip->w4);

    nres_modsub( mr_mip->w4,mr_mip->w6,pa->X);

    nres_modsub( mr_mip->w6,pa->X,mr_mip->w6);
    nres_modsub( mr_mip->w6,pa->X,mr_mip->w6);
    nres_modmult( mr_mip->w8,mr_mip->w6,mr_mip->w2);
    nres_modmult( mr_mip->w1,mr_mip->w5,mr_mip->w1);
    nres_modsub( mr_mip->w2,mr_mip->w1,mr_mip->w5);

/* divide by 2 */
    if (subdiv( mr_mip->w5,2,pa->Y)!=0)
    {
        mr_padd( mr_mip->modulus,mr_mip->w5,pa->Y);
        subdiv( pa->Y,2,pa->Y);
    }
    pa->marker=MR_EPOINT_GENERAL;
    return TRUE;                                      
}

void epoint_copy(epoint *a,epoint *b)
{   
    if (a==b) return;
    copy(a->X,b->X);
    copy(a->Y,b->Y);
    if (a->marker==MR_EPOINT_GENERAL) copy(a->Z,b->Z);
    b->marker=a->marker;
    return;
}

big ecurve_add( epoint *p,epoint *pa)
{  /* pa=pa+p; */
   /* An ephemeral pointer to the line slope is returned */

    if (p==pa) 
    {
        ecurve_double( pa);
        return mr_mip->w8;
    }
    if (pa->marker==MR_EPOINT_INFINITY)
    {
        epoint_copy(p,pa);
        return NULL;
    }
    if (p->marker==MR_EPOINT_INFINITY) 
    {
        return NULL;
    }

    if (!ecurve_padd( p,pa)) ecurve_double( pa);
    return mr_mip->w8;
}

void epoint_negate( epoint *p)
{ /* negate a point */

    if (p->marker==MR_EPOINT_INFINITY) return;

    mr_psub( mr_mip->modulus,p->Y,p->Y);

}

big ecurve_sub( epoint *p,epoint *pa)
{/*	pa-p	*/
    big r;

    if (p==pa)
    {
        convert( 1,mr_mip->w1);
        nres( mr_mip->w1,pa->X);
        nres( mr_mip->w1,pa->Y);
        pa->marker=MR_EPOINT_INFINITY;
        return NULL;
    } 
    if (p->marker==MR_EPOINT_INFINITY) 
    {
        return NULL;
    }

    epoint_negate( p);
    r=ecurve_add( p,pa);
    epoint_negate( p);

    return r;
}

void ecurve_mult( big e,epoint *pa,epoint *pt)
{ /* pt=e*pa; */
    int i,j,n,ch,ce,nb,nbs,nzs;
    epoint *p;
    epoint *table[11];


    if (size(e)==0) 
    { /* multiplied by 0 */
        convert( 1,mr_mip->w1);
        nres( mr_mip->w1,pt->X);
        nres( mr_mip->w1,pt->Y);
        pt->marker=MR_EPOINT_INFINITY;
        return;
    }
    copy(e,mr_mip->w9);
    epoint_norm( pa);
    epoint_copy(pa,pt);

    if (size(mr_mip->w9)<0)
    { /* pt = -pt */
		copy(mr_mip->w9,mr_mip->w9);
		if (mr_mip->w9->len!=0) mr_mip->w9->len^=MR_MSBIT;
        epoint_negate( pt);
    }

    if (size(mr_mip->w9)==1)
    { 
        return;
    }

	mr_pmul( mr_mip->w9,(mr_small)3,mr_mip->w10);
    p=epoint_init();

    epoint_copy(pt,p);

    if (mr_mip->base==mr_mip->base2)
    {
        table[0]=epoint_init();
        epoint_copy(p,table[0]);
        ecurve_double( p);

        for (i=1;i<=10;i++)
        { /* precomputation */
            table[i]=epoint_init();
            epoint_copy(table[i-1],table[i]);
            ecurve_add( p,table[i]);
        }

  /* note that normalising this table doesn't really help */
        nb=logb2( mr_mip->w10);

        for (i=nb-2;i>=1;)
        { /* add/subtract */
            n=mr_naf_window( mr_mip->w9,mr_mip->w10,i,&nbs,&nzs);
            for (j=0;j<nbs;j++)
                ecurve_double( pt);
            if (n>0) ecurve_add( table[n/2],pt);
            if (n<0) ecurve_sub( table[(-n)/2],pt);
            i-=nbs;
            if (nzs)
            {
                for (j=0;j<nzs;j++) ecurve_double( pt);
                i-=nzs;
            }
        }
        for (i=10;i>=0;i--) epoint_free(table[i]);
    }
    else
    { 
        expint( 2,logb2( mr_mip->w10)-1,mr_mip->w11);
        mr_psub( mr_mip->w10,mr_mip->w11,mr_mip->w10);
        subdiv( mr_mip->w11,2,mr_mip->w11);
        while (size(mr_mip->w11) > 1)
        { /* add/subtract method */
            ecurve_double( pt);
            ce=compare(mr_mip->w9,mr_mip->w11); /* e(i)=1? */
            ch=compare(mr_mip->w10,mr_mip->w11); /* h(i)=1? */
            if (ch>=0) 
            {  /* h(i)=1 */
                if (ce<0) ecurve_add( p,pt);
                mr_psub( mr_mip->w10,mr_mip->w11,mr_mip->w10);
            }
            if (ce>=0) 
            {  /* e(i)=1 */
                if (ch<0) ecurve_sub( p,pt);
                mr_psub( mr_mip->w9,mr_mip->w11,mr_mip->w9);  
            }
            subdiv( mr_mip->w11,2,mr_mip->w11);
        }
    }
    epoint_free(p);
}

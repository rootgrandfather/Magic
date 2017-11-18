/*
 *   Elliptic Curve ElGamal Algorithm (EC-ElGamal)
 *
 *   It is assumed that Curve parameters are to be difine in file param.h
 *
 *   The curve is y^2=x^3+Ax+b mod p
 * 
 *	 The file is used to encode and decode message
 *
 *   Ma ChunGuang 2004-03
 */
#include <stdio.h>
#include "miracl.h"
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include "param.h"


#define mr_abs(x)  ((x)<0? (-(x)) : (x))
/*  embed data onto a curve point.	*/


BOOL epoint_set(big x,big y,int cb,epoint *p)
{   
    BOOL valid;


/* find x^3+Ax+B */
    nres( x,p->X);

    nres_modmult( p->X,p->X,mr_mip->w3);

    nres_modmult( mr_mip->w3,p->X,mr_mip->w3);
    if (mr_abs(mr_mip->Asize)==MR_TOOBIG)
        nres_modmult( p->X,mr_mip->A,mr_mip->w1);
    else
	{
	    premult( p->X,mr_mip->Asize,mr_mip->w1);
	    divide( mr_mip->w1,mr_mip->modulus,mr_mip->modulus);
	    if (size(mr_mip->w1)<0) 
		myselect( mr_mip->w1,PLUS,mr_mip->modulus,mr_mip->w1);
	}

    nres_modadd( mr_mip->w3,mr_mip->w1,mr_mip->w3);
    if (mr_abs(mr_mip->Bsize)==MR_TOOBIG)
        nres_modadd( mr_mip->w3,mr_mip->B,mr_mip->w3);
    else
    {
        convert( mr_mip->Bsize,mr_mip->w1);
        nres( mr_mip->w1,mr_mip->w1);
        nres_modadd( mr_mip->w3,mr_mip->w1,mr_mip->w3);
    }
    valid=FALSE;
    if (x!=y)
    { /* compare with y^2 */
        nres( y,p->Y);
        nres_modmult( p->Y,p->Y,mr_mip->w1);
        
        if (compare(mr_mip->w1,mr_mip->w3)==0) valid=TRUE;
    }
    else
    { /* no y supplied - calculate one. Find square root */

        valid=nres_sqroot( mr_mip->w3,p->Y);

    /* check LSB - have we got the right root? */
        redc( p->Y,mr_mip->w1);
        if (remain( mr_mip->w1,2)!=cb) 
            mr_psub( mr_mip->modulus,p->Y,p->Y);

    } 
    if (valid)
    {
        p->marker=MR_EPOINT_NORMALIZED;
        return TRUE;
    }

    return FALSE;
}

 
/*  Send data to another person using ElGamal protocol. Send Hidden_data and
	Random_point to other side. */
int Send_Elgamal(
		char *sPubKey,				/* public key */
		MR_BYTE *syKey,				/* symetric key to be encoded */
		char *Rand_data,			/* cryptography out */
		char *Hidden_data			/* cryptography	out */
		)
{
	char *paramstr;		/* Store curve parameters temporarily */
    epoint *g,*pub;		/* Store basepoint */
    big a,b,p,q,x,y;	/* curve-a,curve-b,modular prime,random value scope,basepoint-x,basepoint-y */
	big rand_value,raw_data,temp;
	epoint	*rand_point,*hidden_point;
    int seed,ep;		/* random seed */
    miracl *mip;		/* circumstance variable */
    int i,in,n;
    mr_unsign32 t,m=1L;


/*	set up the circumstance		*/
    mip=mirsys(3+NUMBITS/MIRACL,0);
    x=mirvar(0);	/* Initial big variables */
    y=mirvar(0);
	q=mirvar(0);
	a=mirvar(0);
	b=mirvar(0);
	p=mirvar(0);
	rand_value=mirvar(0);
	raw_data=mirvar(0);
	temp=mirvar(0);

/*	Get public curve parameters	*/
    mip->IOBASE=16;

	paramstr=BIGPRIME;
    cinstr( p,paramstr);
	paramstr=CURVE_A;
    cinstr( a,paramstr);
	paramstr=CURVE_B;
    cinstr( b,paramstr);
	paramstr=BIGLIMIT;
	cinstr( q,paramstr);
	paramstr=BASEPOINT_X;
    cinstr( x,paramstr);
	paramstr=BASEPOINT_Y;
    cinstr( y,paramstr);
	cinstr( raw_data,syKey);	/* convert plain context into field */

    mip->IOBASE=10;

/* randomise */
	srand((unsigned)time(NULL));
	seed=rand()%10;		/* seed=9,8,7,6,5,4,3,2,1 */
	for(i=0;i<8;i++)
		seed=seed*10+(rand()%10);

/* initialise random number system */
    mr_mip->borrow=0L;
    mr_mip->rndptr=0;
    mr_mip->ira[0]^=seed;
    for (i=1;i<NK;i++)
    { /* fill initialisation vector */
        in=(NV*i)%NK;
        mr_mip->ira[in]=m;      /* note XOR */
        t=m;
        m=seed-m;
        seed=t;
    }

/* deal with mr_miracl */
    ecurve_init(a,b,p);			/* initialise curve */

/* get the basepoint G */
    g=epoint_init();			/* alloc space for point */

    if (!epoint_set(x,y,0,g))	/* initialise point of order q */
    { /* point (x,y) is not on the curve */

/* Free all the space */
		free(a);
		free(b);
		free(p);
		epoint_free(g);
		free(q);
		free(x);
		free(y);
		free(rand_value);
		free(raw_data);
		free(temp);
		free(mip->workspace);
		free(mip->IOBUFF);
		free(mip->modulus);
		free(mip->A);
		free(mip->B);

		free(mip);

		return 0;
    }

/*  create random point to help hide the data  */
	rand_point=epoint_init();
    bigrand(q,rand_value);
    ecurve_mult(rand_value,g,rand_point);	/*	kG	*/
	ep=epoint_get(rand_point,temp,temp);

	cotstr( temp,Rand_data);

/*  embed raw data onto the chosen curve,  Assume raw data is contained in
	least significant ELEMENTs of the field variable and we won't hurt anything
	using the most significant to operate on.  Uses the first root for y value.
*/

/*	embed plain context onto elliptic curve		*/
	hidden_point=epoint_init();

/* get public key of signer */
    pub=epoint_init();
	n=strcspn(sPubKey,",");
	paramstr=sPubKey;
	for(i=0;i<n+1;i++)
		paramstr++;
	ep=atoi(paramstr);
	paramstr=(char *)malloc(sizeof(char)*NUMBITS);
	strcpy(paramstr,sPubKey);
	strtok(paramstr,",");
	cinstr( x,paramstr);
	epoint_set(x,x,ep,pub);
	free(paramstr); 

/*  Create the hiding value using the other person's public key  */
	ecurve_mult(rand_value,pub,hidden_point);
    ep=epoint_get(hidden_point,temp,temp); /* compress point */
	mr_padd(raw_data,temp,temp);
	cotstr( temp,Hidden_data);

/*	Free all the space	*/
	free(a);
	free(b);
	free(p);
	epoint_free(g);
	free(q);
	free(x);
	free(y);
	free(rand_value);
	free(raw_data);
	free(rand_point);
	free(temp);
	free(mip->workspace);
	free(mip->IOBUFF);
	free(mip->modulus);
	free(mip->A);
	free(mip->B);

	free(mip);

	return 1;

}


/*  Recieve data from another person using ElGamal protocol. We get
	Hidden_data and Random_point and output raw_data. */

int Receive_Elgamal(
		char *sPriKey ,			/* private key in */
		char *Rand_data,
		char *Hidden_data,		/* cryptography in */
		MR_BYTE *syKey			/* symetric key to be encoded out */
		)
{
	int ep;
	miracl *mip;
	big a,b,p,q,x,y,raw_data,rand_value,hidden_value,tmp;
	char *paramstr;
	epoint	*g,*raw_point,*rand_point;


/*	set up the circumstance		*/
    mip=mirsys(3+NUMBITS/MIRACL,0);
    x=mirvar(0);	/* Initial big variables */
    y=mirvar(0);
	q=mirvar(0);
	a=mirvar(0);
	b=mirvar(0);
	p=mirvar(0);
	tmp=mirvar(0);
	raw_data=mirvar(0);
	rand_value=mirvar(0);
	hidden_value=mirvar(0);

/*	Get public curve parameters	*/
    mip->IOBASE=16;
	paramstr=BIGPRIME;
    cinstr( p,paramstr);
	paramstr=CURVE_A;
    cinstr( a,paramstr);
	paramstr=CURVE_B;
    cinstr( b,paramstr);
	paramstr=BIGLIMIT;
	cinstr( q,paramstr);
	paramstr=BASEPOINT_X;
    cinstr( x,paramstr);
	paramstr=BASEPOINT_Y;
    cinstr( y,paramstr);
    mip->IOBASE=10;

	cinstr( rand_value,Rand_data);
	cinstr( hidden_value,Hidden_data);

/* deal with mr_miracl */
    ecurve_init(a,b,p);			/* initialise curve */

/* get the basepoint G */
    g=epoint_init();			/* alloc space for point */

    if (!epoint_set(x,y,0,g))	/* initialise point of order q */
    {/* point (x,y) is not on the curve */

/*	Free all the space	*/
		free(a);
		free(b);
		free(p);
		epoint_free(g);
		free(q);
		free(x);
		free(y);
		free(rand_value);
		free(raw_data);
		free(tmp);
		free(hidden_value);
		free(mip->workspace);
		free(mip->IOBUFF);
		free(mip->modulus);
		free(mip->A);
		free(mip->B);

		free(mip);

		return 0;
    }

/*  compute hidden point using my private key and the random point  */
	raw_point=epoint_init();
	rand_point=epoint_init();
	epoint_set(rand_value,rand_value,1,rand_point);	/* depress */
	cinstr( tmp,sPriKey);
	ecurve_mult(tmp,rand_point,raw_point);
	ep=epoint_get(raw_point,raw_data,raw_data);		/* compress */
	mr_psub(hidden_value,raw_data,tmp);
	mr_mip->IOBASE=16;
	cotstr(tmp,syKey);
	mr_mip->IOBASE=10;

/*	free space	*/
	free(a);
	free(b);
	free(p);
	free(q);
	free(x);
	free(y);
	free(raw_data);
	free(tmp);
	epoint_free(g);
	free(raw_point);
	free(rand_point);
	free(mip->workspace);
	free(mip->IOBUFF);
	free(mip->modulus);
	free(mip->A);
	free(mip->B);

	free(mip);

	return 1;
}

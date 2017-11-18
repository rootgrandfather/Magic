/*
 *   Elliptic Curve Digital Signature Algorithm (ECDSA)
 *
 *   It is assumed that Curve parameters are to be found in file common.ecs
 *
 *   The curve is y^2=x^3+Ax+b mod p
 * 
 *	 The file is used to generate private and public key
 *
 *   Ma ChunGuang 2004-03
 */

#include <stdlib.h>
#include <stdio.h>
#include "param.h"
#include "miracl.h"
#include "string.h"
#include "time.h"

/* Generate Public Key */
int ECC_GenKey(
		char *sPriKey,		/* Private Key out */
		char *sPubKey		/* Public key out */
		)
{
	char *paramstr;			/* Store curve parameters temporarily */
	char tmp[20];
    epoint *g;				/* Store basepoint */
    big a,b,p,q,x,y,d;		/* curve-a,curve-b,modular prime,random value scope,basepoint-x,basepoint-y,private key	*/
    int seed;				/* random seed	*/
    miracl *mip;			/* circumstance variable */
    int i,in,n,ep;
    mr_unsign32 t,m=1L;
    

/*	set up the circumstance		*/
    mip=mirsys(3+NUMBITS/MIRACL,0);
    a=mirvar(0);	/* Initial big variables */
    b=mirvar(0);
    p=mirvar(0);
    q=mirvar(0);
    x=mirvar(0);
    y=mirvar(0);
    d=mirvar(0);

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

/* randomise */
	srand((unsigned)time(NULL));
	seed=rand()%10;		/* seed=1,2,3,4,5,6,7,8,9 */
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

/*	deal with mr_miracl	*/
    ecurve_init(a,b,p);  /* initialise curve */

    g=epoint_init();	/* alloc space for point */

    if (!epoint_set(x,y,0,g)) /* initialise point of order q */
    { /* point (x,y) is not on the curve */

/*	Free all the space	*/
		free(a);
		free(b);
		free(p);
		epoint_free(g);
		free(q);
		free(x);
		free(y);
		free(d);
		free(mip->workspace);
		free(mip->IOBUFF);
		free(mip->modulus);
		free(mip->A);
		free(mip->B);

		free(mip);

		return 0;
    }

/* generate public/private keys */

    bigrand(q,d);			/* random big number is d */
    ecurve_mult(d,g,g);		/* public key=private key * basepoint */
    epoint_norm( g);

    mip->IOBASE=10;

    ep=epoint_get(g,x,x);	 /* compress point */

	n=cotstr( x,sPubKey);
	strcat(sPubKey,",");
	sprintf(tmp,"%d",ep);
	strcat(sPubKey,tmp);
    n=cotstr( d,sPriKey);
	mip->IOBASE=16;

/* depose space */
	free(a);
	free(b);
	free(p);
	free(q);
	free(x);
	free(y);
	free(d);
	epoint_free(g);
	free(mip->workspace);
	free(mip->IOBUFF);
	free(mip->modulus);
	free(mip->A);
	free(mip->B);
	free(mip);

    return 1;
}

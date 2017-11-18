/*
 *   main MIRACL header - miracl.h.
 *
 *	 The file includes all variables & functions declaration
 *
 *	 The only global variable is mr_mip
 */

#include <stdio.h>
#include "mirdef.h"

/* some useful definitions */

#define forever for(;;)   
#define FALSE 0
#define TRUE 1
#define OFF 0
#define ON 1
#define PLUS 1
#define MINUS (-1)


/* big and flash variables consist of an encoded length, *
 * and an array of mr_smalls containing the digits       */

typedef int BOOL;

typedef unsigned mr_utype mr_small;  //mr_small=unsigned int 
typedef unsigned mr_dltype mr_large;  //mr_large=unsigned _int64


#define MR_DIV(a,b)    ((a)/(b))
#define MR_REMAIN(a,b) ((a)%(b))
#define MR_LROUND(a)   ((a))

/* This a structure to store big integers	*/
struct bigtype
{
    mr_unsign32 len;
    mr_small *w;
};                

typedef struct bigtype *big;

/*	define a series of operations	*/
#define BIG(x,xt,xg,s) mr_small xg[s]; struct bigtype xt={s,xg}; big x=&xt;

typedef big flash;	

#define MR_MSBIT ((mr_unsign32)1<<31)/*	minus signal bit*/

#define MR_OBITS (MR_MSBIT-1)			

#define MR_TOOBIG (1<<(MR_IBITS-2))


/* Marsaglia & Zaman Random number generator */
/*         constants      alternatives       */
#define NK   37           /* 21 */
#define NJ   24           /*  6 */
#define NV   14           /*  8 */


#define MR_TOP(x) (*(((mr_small *)&(x))+1))
#define MR_BOT(x) (*(((mr_small *)&(x))))


/* ECDSA signature structure */
/*typedef struct{
	big	r;
	big	s;
}ECC_SIGNATURE;
*/

/* secure hash Algorithm structure */
typedef struct {
mr_unsign32 length[2];
mr_unsign32 h[8];
mr_unsign32 w[80];
} sha256;

typedef sha256 sha;


/* Elliptic curve point status */

#define MR_EPOINT_GENERAL    0
#define MR_EPOINT_NORMALIZED 1
#define MR_EPOINT_INFINITY   2


/* Elliptic Curve epoint structure. Uses projective (X,Y,Z) co-ordinates */

typedef struct {
big X;
big Y;
big Z;
int marker;	/* MR_EPOINT_GENERAL-0  MR_EPOINT_NORMALIZED-1 MR_EPOINT_INFINITY-2*/
} epoint;


/* main MIRACL instance structure */

typedef struct
{
	mr_small base;       /* number base     */
	mr_small apbase;     /* apparent base   */
	int   pack;          /* packing density */
	int   lg2b;          /* bits in base    */
	mr_small base2;      /* 2^mr_lg2b       */

	int   nib;           /* length of bigs  */
	BOOL  check;         /* overflow check  */

	mr_unsign32 ira[NK];  /* random number...   */
	int         rndptr;   /* ...array & pointer */
	mr_unsign32 borrow;
				/* Montgomery constants */
	mr_small ndash;
	big modulus;
						   /* Elliptic Curve details */
	big A,B,C;
	int Asize,Bsize;

	big w0;            /* workspace bigs  */
	big w1,w2,w3,w4;
	big w5,w6,w7;
	big w8,w9,w10,w11;
	big w12,w13,w14,w15;
	big w16,w17,w18;

	/* User modifiables */

	char *IOBUFF;		/* i/o buffer    */
	int  IOBSIZ;		/* size of i/o buffer */
	int  IOBASE;		/* base for input and output */
	int  INPLEN;		/* input length              */

	int size;
	char *workspace;
} miracl;

miracl *mr_mip;	/* MIRACL's one and only global variable */


/* Function definitions  */

/* Caculate hash value */
void hashing(MR_BYTE *context,MR_DWORD len,big hash);

/* Generate private and public key	*/
int ECC_GenKey(char *sPriKey,char *vPubKey);




/*	EC-ElGamal cryptographic method	*/
int Send_Elgamal(
		char *sPubKey,				/* public key */
		MR_BYTE *syKey,				/* symetric key to be encoded */
		char *Rand_data,			/* cryptography out */
		char *Hidden_data			/*cryptography out */
		);
int Receive_Elgamal(
		char *sPriKey ,				/* private key in */
		char *Rand_data,
		char *Hidden_data,          /* cryptography	in */
		MR_BYTE *syKey				/* symetric key to be encoded out */
		);


/* Group 0 - Internal routines */

mr_small mr_setbase( mr_small);	/* Set the base of caculation */
void  mr_lzero(big);			/* strip leading zeros from big number  */
int  mr_padd( big,big,big);		/* a+b=c mod p */
int  mr_psub( big,big,big);		/* a-b=c mod p */
int  mr_pmul( big,mr_small,big);
mr_small mr_sdiv( big,mr_small,big);	
void  mr_shift( big,int,big); 
int   mr_window( big,int,int *,int *); /*	returns sliding window value to calculate w=x^y mod z	*/ 
int   mr_naf_window( big,big,int,int *,int *); /*	returns sliding window value used in scalar-multiplication	*/
mr_small muldiv(mr_small,mr_small,mr_small,mr_small,mr_small *);
mr_small muldvm(mr_small,mr_small,mr_small,mr_small *); 


/* Group 1 - General purpose, I/O and basic arithmetic routines  */

void  zero(flash);
void  convert( int,big);
flash mirvar( int);					/* initial a big number	*/
flash mirvar_mem( char *,int);	
miracl *mirsys(int,mr_small);		/* set up circumstance */
int   getdig( big,int);				/* extract a packed digit */      
void  putdig( int,big,int);			/* insert a digit into a packed word */
void  copy(flash,flash);  
int   size(big);
int   compare(big,big);
void  myselect( big x,int d,big y,big z); /* d is + or -,perform required add or subtract operation */
void  premult( big,int,big);		/* premultiply a big number by an int z=x.n */
int   subdiv( big,int,big);			/* subdivide a big number by an int   z=x/n */ 
int   remain( big,int);				/* return integer remainder when x divided by n */ 
int  bytes_to_big( int,char *,big);	
int  multiply( big,big,big);
void  divide( big,big,big);  
int   instr( flash,char *);			/* returns length in digits */
int   otstr( flash,char *);			/* output a big number */
int   cinstr( flash,char *);		/* input big number in base IOBASE */
int   cotstr( flash,char *);		/* output a big number in base IOBASE */


/* Group 2 - Advanced arithmetic routines */

int   xgcd( big,big,big,big,big);	/*	z = x.xd + y.yd = gcd(x,y)	*/
int   logb2( big);					/* returns number of bits in x */
void  expint( int,int,big);			/* sets x=b^n */
void  bigrand( big,big);			/* Generate a big random number	*/


/* Montgomery stuff	used in Montgomery method of multiplicaion	*/

void  nres( big,big);        
void  redc( big,big);        
void  nres_negate( big,big);
void  nres_modadd( big,big,big);    
void  nres_modsub( big,big,big);    
void  nres_modmult( big,big,big);    
int   nres_moddiv( big,big,big);     
void  nres_powmod( big,big,big);     
void  nres_powltr( int,big,big);     
BOOL  nres_sqroot( big,big);


/* elliptic curve stuff */

void ecurve_init( big,big,big);
big  ecurve_add( epoint *,epoint *);
big  ecurve_sub( epoint *,epoint *);
void ecurve_mult( big,epoint *,epoint *);

epoint* epoint_init( );
BOOL epoint_set( big,big,int,epoint*);
int  epoint_norm( epoint *);
void epoint_free(epoint *);
void epoint_copy(epoint *,epoint *);
void epoint_negate( epoint *);
int  epoint_get( epoint*,big,big);


/* Group 3 - Floating-slash routines */

mr_small muldvd(mr_small,mr_small,mr_small,mr_small *); 

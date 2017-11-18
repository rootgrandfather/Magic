/*
 *   Core module - contains initialisation code and general purpose 
 *   utilities.
 *   mrcore.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "miracl.h"


/* C only versions of muldiv/muldvd/muldvd2/muldvm */
/* Note that mr_large should be twice the size of mr_small */

mr_small muldiv(mr_small a,mr_small b,mr_small c,mr_small m,mr_small *rp)
{
    mr_small q;
    mr_large p=(mr_large)a*b+c;
    q=(mr_small)(MR_LROUND(p/m));
    *rp=(mr_small)(p-(mr_large)q*m);
    return q;
}

mr_small muldvm(mr_small a,mr_small c,mr_small m,mr_small *rp)
{
    mr_small q;
    mr_large dble;
    MR_BOT(dble)=c;
    MR_TOP(dble)=a;
    q=(mr_small)(dble/m);
    *rp=(mr_small)(dble-(mr_large)q*m);
    return q;
}

mr_small muldvd(mr_small a,mr_small b,mr_small c,mr_small *rp)
{
    mr_large dble=(mr_large)a*b+c;
    *rp=(mr_small)dble;
    return MR_TOP(dble);
}

static mr_small brand()
{ /* Marsaglia & Zaman random number generator */
    int i,k;
    mr_unsign32 pdiff,t;
    mr_small r;


    if (mr_mip->lg2b>32)
    { /* underlying type is > 32 bits. Assume <= 64 bits */
        mr_mip->rndptr+=2;
        if (mr_mip->rndptr<NK-1)
        {
            r=(mr_small)mr_mip->ira[mr_mip->rndptr];
			if (mr_mip->lg2b-32>0) r<<=mr_mip->lg2b-32;
			else r>>=mr_mip->lg2b-32;

            r+=(mr_small)mr_mip->ira[mr_mip->rndptr+1];
            return r;
        }
    }
    else
    {
        mr_mip->rndptr++;
        if (mr_mip->rndptr<NK) return (mr_small)mr_mip->ira[mr_mip->rndptr];
    }
    mr_mip->rndptr=0;
    for (i=0,k=NK-NJ;i<NK;i++,k++)
    { /* calculate next NK values */
        if (k==NK) k=0;
        t=mr_mip->ira[k];
        pdiff=t - mr_mip->ira[i] - mr_mip->borrow;
        if (pdiff<t) mr_mip->borrow=0;
        if (pdiff>t) mr_mip->borrow=1;
        mr_mip->ira[i]=pdiff; 
    }
    if (mr_mip->lg2b>32)
    { /* double up */
        r=(mr_small)mr_mip->ira[0];
		if (mr_mip->lg2b-32>0) r<<=mr_mip->lg2b-32;
		else r>>=mr_mip->lg2b-32;
        r+=(mr_small)mr_mip->ira[1];
        return r;
    }
    else return (mr_small)(mr_mip->ira[0]);
}

void bigrand( big w,big x)
{  /*  generate a big random number 0<=x<w  */
    int m;
    mr_small r;

 /*   decr( w,2,w);  */
    m=0;
    zero(mr_mip->w0);

    do
    { /* create big rand piece by piece */
        m++;
        mr_mip->w0->len=m;
        r=brand();
        if (mr_mip->base==0) mr_mip->w0->w[m-1]=r;
        else                 mr_mip->w0->w[m-1]=MR_REMAIN(r,mr_mip->base);
    } while (compare(mr_mip->w0,w)<0);
    mr_lzero(mr_mip->w0);
    divide( mr_mip->w0,w,w);

    copy(mr_mip->w0,x);
}

mr_small mr_setbase( mr_small nb)
{  /* set base. Pack as many digits as  *
    * possible into each computer word  */
    mr_small temp;
    BOOL fits;
    int bits;

    fits=FALSE;
    bits=MIRACL;
    while (bits>1) 
    {
        bits/=2;
        temp=((mr_small)1<<bits);
        if (temp==nb)
        {
            fits=TRUE;
            break;
        }
        if (temp<nb || (bits%2)!=0) break;
    }
    if (fits)
    {
        mr_mip->apbase=nb;
        mr_mip->pack=MIRACL/bits;
        mr_mip->base=0;
        return 0;
    }
    mr_mip->apbase=nb;
    mr_mip->pack=1;
    mr_mip->base=nb;
    if (mr_mip->base==0) return 0;
    temp=MR_DIV(MAXBASE,nb);
    while (temp>=nb)
    {
        temp=MR_DIV(temp,nb);
        mr_mip->base*=nb;
        mr_mip->pack++;
    }

    return 0;

}

void zero(flash x)
{ /* set big/flash number to zero */
    int i,n;
    mr_small *g;

    if (x==NULL) return;

    n=(x->len&MR_OBITS);
    g=x->w;

    for (i=0;i<n;i++)
        g[i]=0;

    x->len=0;
}

void convert( int n ,big x)
{  /*  convert integer n to big number format  */
    int m;
    mr_unsign32 s;

    zero(x);
    if (n==0) return;
    s=0;
    if (n<0)
    {
        s=MR_MSBIT;
        n=(-n);
    }
    m=0;
    if (mr_mip->base==0)
        x->w[m++]=(mr_small)n;
    else while (n>0)
    {
        x->w[m++]=MR_REMAIN((mr_small)n,mr_mip->base);
        n/=mr_mip->base;
    }
    x->len=(m|s);
}

flash mirvar( int iv)
{ /* initialize big/flash number */
    flash x;
    int align;
    char *ptr;

/* OK, now I control alignment.... */

/* Allocate space for big, the length, the pointer, and the array */
/* Do it all in one memory allocation - this is quicker */
/* Ensure that the array has correct alignment */

    x=(big)calloc(mr_mip->size,1);
    if (x==NULL) 
	{
		free(x);
		return NULL;
	}

    ptr=(char *)&x->w;
    align=(unsigned int)(ptr+sizeof(mr_small *))%sizeof(mr_small);   

    x->w=(mr_small *)(ptr+sizeof(mr_small *)+sizeof(mr_small)-align);   

    if (iv!=0) convert( iv,x);
    return x;
}

flash mirvar_mem( char *mem,int index)
{ /* initialize big/flash number from pre-allocated memory */
    flash x;
    int align;
    char *ptr;
 
    x=(big)&mem[mr_mip->size*index];
    ptr=(char *)&x->w;
    align=(unsigned int)(ptr+sizeof(mr_small *))%sizeof(mr_small);   
    x->w=(mr_small *)(ptr+sizeof(mr_small *)+sizeof(mr_small)-align);   

    return x;
}

miracl *mirsys(int nd,mr_small nb)
{  /*  Initialize MIRACL system to   *
    *  use numbers to base nb, and   *
    *  nd digits or (-nd) bytes long */
    int i;
    mr_small b;

/*** Multi-Threaded support ***/
    mr_mip=(miracl *)calloc(1,sizeof(miracl));

    if (mr_mip==NULL) return NULL;
    /* digest hardware configuration */

    if (nb==1 || nb>MAXBASE)			
    {
		free(mr_mip);
        return NULL;
    }

    mr_setbase( nb);
    b=mr_mip->base;

    mr_mip->lg2b=0;
    mr_mip->base2=1;
    if (b==0)
    {
        mr_mip->lg2b=MIRACL;
        mr_mip->base2=0;
    }
    else while (b>1)
    {
        b=MR_DIV(b,2);
        mr_mip->lg2b++;
        mr_mip->base2*=2;
    }
    mr_mip->nib=(nd-1)/mr_mip->pack+1;
    if (mr_mip->nib<2) mr_mip->nib=2;
    mr_mip->size=sizeof(mr_unsign32)+sizeof(mr_small *)+(mr_mip->nib+1)*sizeof(mr_small);     

    mr_mip->check=ON;
    mr_mip->IOBASE=10;   /* defaults */
    mr_mip->INPLEN=0;
    mr_mip->IOBSIZ=1024;
    mr_mip->IOBUFF=(char *)calloc(1025,1);
    if (mr_mip->IOBUFF==NULL)
	{
		free(mr_mip);
		return NULL;
	}
    mr_mip->IOBUFF[0]='\0';

/* quick start for rng. irand(.) should be called first before serious use.. */

    mr_mip->ira[0]=0x55555555;
    mr_mip->ira[1]=0x12345678;

    for (i=2;i<NK;i++) 
        mr_mip->ira[i]=mr_mip->ira[i-1]+mr_mip->ira[i-2]+0x1379BDF1;
    mr_mip->rndptr=NK;
    mr_mip->borrow=0;

    mr_mip->nib=2*mr_mip->nib+1;

    if (mr_mip->nib!=(mr_mip->nib&(MR_OBITS)) || mr_mip->nib>MR_TOOBIG)			
    {
        mr_mip->nib=(mr_mip->nib-1)/2;
        return mr_mip;
    }
    mr_mip->modulus=NULL;
    mr_mip->A=NULL;
    mr_mip->B=NULL;
    mr_mip->C=NULL;

    mr_mip->workspace=calloc(mr_mip->size*23,1);
    if (mr_mip->workspace==NULL)
	{
		free(mr_mip->IOBUFF);
		free(mr_mip);
		return NULL;
	}
    
    mr_mip->nib=(mr_mip->nib-1)/2;

/* allocate memory for workspace variables */

    mr_mip->w0=mirvar_mem( mr_mip->workspace,0);  /* double length */
    mr_mip->w1=mirvar_mem( mr_mip->workspace,2);
    mr_mip->w2=mirvar_mem( mr_mip->workspace,3);
    mr_mip->w3=mirvar_mem( mr_mip->workspace,4);
    mr_mip->w4=mirvar_mem( mr_mip->workspace,5);
    mr_mip->w5=mirvar_mem( mr_mip->workspace,6);  /* double length */
    mr_mip->w6=mirvar_mem( mr_mip->workspace,8);  /* double length */
    mr_mip->w7=mirvar_mem( mr_mip->workspace,10); /* double length */
    mr_mip->w8=mirvar_mem( mr_mip->workspace,12);
    mr_mip->w9=mirvar_mem( mr_mip->workspace,13);
    mr_mip->w10=mirvar_mem( mr_mip->workspace,14);
    mr_mip->w11=mirvar_mem( mr_mip->workspace,15);
    mr_mip->w12=mirvar_mem( mr_mip->workspace,16);
    mr_mip->w13=mirvar_mem( mr_mip->workspace,17);
    mr_mip->w14=mirvar_mem( mr_mip->workspace,18);
    mr_mip->w15=mirvar_mem( mr_mip->workspace,19);
    mr_mip->w16=mirvar_mem( mr_mip->workspace,20);
    mr_mip->w17=mirvar_mem( mr_mip->workspace,21);
    mr_mip->w18=mirvar_mem( mr_mip->workspace,22);

    return mr_mip;
} 

void mr_lzero(big x)
{  /*  strip leading zeros from big number  */
    mr_unsign32 s;
    int m;
    s=(x->len&(MR_MSBIT));
    m=(int)(x->len&(MR_OBITS));
    while (m>0 && x->w[m-1]==0)
        m--;
    x->len=m;
    if (m>0) x->len|=s;
}

int getdig( big x,int i)
{  /* extract a packed digit */
    int k;
    mr_small n;

    i--;
    n=x->w[i/mr_mip->pack];

    if (mr_mip->pack==1) return (int)n;
    k=i%mr_mip->pack;
    for (i=1;i<=k;i++)
        n=MR_DIV(n,mr_mip->apbase);  
    return (int)MR_REMAIN(n,mr_mip->apbase);
}

void putdig( int n,big x,int i)
{  /* insert a digit into a packed word */
    int j,k,lx;
    mr_small m,p;
    mr_unsign32 s;

    s=(x->len&(MR_MSBIT));
    lx=(int)(x->len&(MR_OBITS));
    m=getdig( x,i);
    p=n;
    i--;
    j=i/mr_mip->pack;
    k=i%mr_mip->pack;
    for (i=1;i<=k;i++)
    {
        m*=mr_mip->apbase;
        p*=mr_mip->apbase;
    }
    if (j>=mr_mip->nib && (mr_mip->check || j>=2*mr_mip->nib))			
        return;
    x->w[j]=(x->w[j]-m)+p;
    if (j>=lx) x->len=((j+1)|s);
    mr_lzero(x);
}

void copy(flash x,flash y)
{  /* copy x to y: y=x  */
    int i,nx,ny;
    mr_small *gx,*gy;


    if (x==y) return;

    if (x==NULL)
    { 
        zero(y);
        return;
    }

    ny=(y->len&(MR_OBITS));
    nx=(x->len&(MR_OBITS));

    gx=x->w;
    gy=y->w;

    for (i=nx;i<ny;i++)
        gy[i]=0;
    for (i=0;i<nx;i++)
        gy[i]=gx[i];
    y->len=x->len;

	gx=gy=NULL;
}


void mr_shift( big x,int n,big w)
{ /* set w=x.(mr_base^n) by shifting */
    mr_unsign32 s;
    int i,bl;
    mr_small *gw=w->w;

    copy(x,w);
    if (w->len==0 || n==0) return;

    s=(w->len&(MR_MSBIT));
    bl=(int)(w->len&(MR_OBITS))+n;
    if (bl<=0)
    {
        zero(w);
        return;
    }
    if (bl>mr_mip->nib && mr_mip->check)
		return;

    if (n>0)
    {
        for (i=bl-1;i>=n;i--)
            gw[i]=gw[i-n];
        for (i=0;i<n;i++)
            gw[i]=0;
    }
    else
    {
        n=(-n);
        for (i=0;i<bl;i++)
            gw[i]=gw[i+n];
        for (i=0;i<n;i++)
            gw[bl+i]=0;
    }
    w->len=(bl|s);
}

int size(big x)
{  /*  get size of big number;  convert to *
    *  integer - if possible               */
    int n,m;
    mr_unsign32 s;
    if (x==NULL) return 0;
    s=(x->len&MR_MSBIT);
    m=(int)(x->len&MR_OBITS);
    if (m==0) return 0;
    if (m==1 && x->w[0]<(mr_small)MR_TOOBIG) n=(int)x->w[0];
    else                                     n=MR_TOOBIG;
    if (s==MR_MSBIT) return (-n);
    return n;
}

int compare(big x,big y)
{  /* compare x and y: =1 if x>y  =-1 if x<y *
    *  =0 if x=y                             */
    int m,n,sig;
    mr_unsign32 sx,sy;

    if (x==y) return 0;
    sx=(x->len&MR_MSBIT);
    sy=(y->len&MR_MSBIT);
    if (sx==0) sig=PLUS;
    else       sig=MINUS;
    if (sx!=sy) return sig;
    m=(int)(x->len&MR_OBITS);
    n=(int)(y->len&MR_OBITS);
    if (m>n) return sig;
    if (m<n) return -sig;
    while (m>0)
    { /* check digit by digit */
        m--;  
        if (x->w[m]>y->w[m]) return sig;
        if (x->w[m]<y->w[m]) return -sig;
    }
    return 0;
}


/* routines to support sliding-windows exponentiation *
 * in various contexts */

int mr_window( big x,int i,int *nbs,int * nzs)
{ /* returns sliding window value, max. of 5 bits,         *
   * starting at i-th bit of big x. nbs is number of bits  *
   * processed, nzs is the number of additional trailing   *
   * zeros detected. Returns valid bit pattern 1x..x1 with *
   * no two adjacent 0's. So 10101 will return 21 with     *
   * nbs=5, nzs=0. 11001 will return 3, with nbs=2, nzs=2, *
   * having stopped after the first 11..  */

    int j,r,w;
    w=5;

/* check for leading 0 bit */

    *nbs=1;
    *nzs=0;
    if ((x->w[i/mr_mip->lg2b] & ((mr_small)1<<(i%mr_mip->lg2b))) <=0) return 0;

/* adjust window size if not enough bits left */
   
    if (i-w+1<0) w=i+1;

    r=1;
    for (j=i-1;j>i-w;j--)
    { /* accumulate bits. Abort if two 0's in a row */
        (*nbs)++;
        r*=2;
    if ((x->w[j/mr_mip->lg2b] & ((mr_small)1<<(j%mr_mip->lg2b))) >0) r+=1;

        if (r%4==0)
        { /* oops - too many zeros - shorten window */
            r/=4;
            *nbs-=2;
            *nzs=2;
            break;
        }
    }
    if (r%2==0)
    { /* remove trailing 0 */
        r/=2;
        *nzs=1;
        (*nbs)--;
    }
    return r;
}

int mr_naf_window( big x,big x3,int i,int *nbs,int *nzs)
{ /* returns sliding window value, max of 5 bits           *
   * starting at i-th bit of x. nbs is number of bits      *
   * processed. nzs is number of additional trailing       *    
   * zeros detected. x and x3 (which is 3*x) are           *
   * combined to produce the NAF (non-adjacent form)       *
   * So if x=11011(27) and x3 is 1010001, the LSB is       *
   * ignored and the value 100T0T (32-4-1=27) processed,   *
   * where T is -1. Note x.P = (3x-x)/2.P. This value will *
   * return +7, with nbs=4 and nzs=1, having stopped after *
   * the first 4 bits. Note in an NAF non-zero elements    *
   * are never side by side, so 10T10T won't happen        *
   * NOTE: return value n zero or odd, -21 <= n <= +21     */

    int nb,j,r,w;
    BOOL last;
    w=5;

 /* get first bit */
	nb=0;
    if ((x3->w[i/mr_mip->lg2b] & ((mr_small)1<<(i%mr_mip->lg2b))) >0) nb=1;
    if ((x->w[i/mr_mip->lg2b] & ((mr_small)1<<(i%mr_mip->lg2b))) >0) nb-=1;

    *nbs=1;
    *nzs=0;
    if (nb==0) return 0;
    last=FALSE;
    if (i<=w) 
    {
        w=i;
        last=TRUE;
    }

    if (nb>0) r=1;
    else      r=(-1);

    for (j=i-1;j>i-w;j--)
    { /* scan the bits */
        (*nbs)++;
        r*=2;
		nb=0;
		if ((x3->w[j/mr_mip->lg2b] & ((mr_small)1<<(j%mr_mip->lg2b))) >0) nb=1;
	    if ((x->w[j/mr_mip->lg2b] & ((mr_small)1<<(j%mr_mip->lg2b))) >0) nb-=1;

        if (nb==0) continue;
        if (nb>0) r+=1;
        if (nb<0) r-=1;
           
    } 
    if (!last && r%2!=0) (*nzs)++;
    while (r%2==0)
    { /* remove trailing zeros */
        r/=2;
        (*nzs)++;
        (*nbs)--;
    }     
    return r;
}

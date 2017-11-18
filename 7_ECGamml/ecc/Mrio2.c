/*
 *   I/O routines 2. 
 *   mrio2.c
 */

#include <stdio.h>
#include "miracl.h"


static void cbase( big x,mr_small oldbase,big y)
{  /*  change radix of x from oldbase to base  */
    int i,s;
    mr_small n;
    BOOL done;


    if (mr_mip->base==oldbase)
    {
        copy(x,y);
        return;
    }

	s=MINUS;
	if ((x->len&(MR_MSBIT))==0) s=PLUS;

    copy(x,mr_mip->w1);
    done=TRUE;

	if (mr_mip->w1->len!=0)
		mr_mip->w1->len&=MR_OBITS;

    forever
    {
        zero(mr_mip->w6);
        convert( 1,mr_mip->w0);

        for (i=0;i<(int)mr_mip->w1->len;i++)  
        { /* this is a bit slow - but not time critical */


            mr_pmul( mr_mip->w0,mr_mip->w1->w[i],mr_mip->w5);

			myselect( mr_mip->w6,PLUS,mr_mip->w5,mr_mip->w6);
            if (oldbase==0)
            { /* bit of a frig! */
				n=1<<(MIRACL/2);
                mr_pmul( mr_mip->w0,n,mr_mip->w0);
                mr_pmul( mr_mip->w0,n,mr_mip->w0);
            }
            else mr_pmul( mr_mip->w0,oldbase,mr_mip->w0);
        }
        if (done) break;
    }

    copy(mr_mip->w6,y);

	if (y->len!=0)
	{
		if (s<0) x->len|=MR_MSBIT;
		else     x->len&=MR_OBITS;
	}
}

int cinstr( flash x,char *string)
{  /*  input big number in base IOBASE  */
    mr_small newb,oldb,b;
    mr_unsign32 lx;
    int ipt;

    newb=mr_mip->IOBASE;
    oldb=mr_mip->apbase;
    mr_setbase( newb); /* temporarily change base ... */
    b=mr_mip->base;
    mr_mip->check=OFF;
    ipt=instr( mr_mip->w5,string); /* ... and get number  */

    mr_mip->check=ON;
    lx=(mr_mip->w5->len&MR_OBITS);

    if ((int)lx>mr_mip->nib)			
    { /* numerator or denominator too big */
        return 0;
    }
    mr_setbase( oldb);      /* restore original base */

    cbase( mr_mip->w5,b,x);

    return ipt;
}   

int cotstr( flash x,char *string)
{  /*  output a big number in base IOBASE  */
    mr_small newb,oldb,b;
    int ipt;


    newb=mr_mip->IOBASE;
    oldb=mr_mip->apbase;
    b=mr_mip->base;
    mr_setbase( newb);   /* temporarily change base ... */
    mr_mip->check=OFF;

    cbase( x,b,mr_mip->w5);   /* number may get bigger !     */   
    mr_mip->check=ON;

    ipt=otstr( mr_mip->w5,string); /*..... and output number  */ 
    zero(mr_mip->w5);
    mr_setbase( oldb);
    return ipt;
}

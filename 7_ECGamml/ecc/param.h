/*
 *	 param.h
 *
 *   y^2=x^3+Ax+b mod p
 *
 *   The file includes all the public parameters used in the all files
 *
 *   Ma Chun-Guang 2004-03
 */

#define	NUMBITS	192			/* This is the key length */
#define BIGPRIME "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF"	/* Modular p is a big prime */
#define CURVE_A "-3"		/* The first parameter of curve */
#define CURVE_B "64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1" /* The second param of curve */
#define BIGLIMIT "FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831" /* The scope of random value */
#define BASEPOINT_X "188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012" /* The x-cordinate of basepoint */
#define BASEPOINT_Y "07192B95FFC8DA78631011ED6B24CDD573F977A11E794811" /* The y-cordinate of basepoint */

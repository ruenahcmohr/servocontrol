/*
             CREATOR: DAn williams
                DATE: sep 24 2000
         DESCRIPTION: finally, my general header file
            FILENAME: danStuff.h
             STARTED: sep 24 2000
1st VERSION FINISHED: day after never
modified nov 26 2007
added chomp, jan 12 2012
added chricmp, othercase, strupper, strlower, Apr 2, 2012

*/
#ifndef __danstuff
#define __danstuff

#include <string.h>

#define EXIT_OK 1
#define EXIT_FAIL -1

#define IsHigh(BIT,BYTE)    ((BYTE & (1<<BIT)) != 0)
#define IsLow(BIT,BYTE)     ((BYTE & (1<<BIT)) == 0)
#define SetBit(BIT,BYTE)     BYTE |= (1<<BIT)
#define ClearBit(BIT,BYTE)   BYTE &= ~(1<<BIT)

#define ABS(a)                ((a) < 0 ? -(a) : (a))
#define SIGN(x)               (x)==0?0:(x)>0?1:-1
#define SQR(x)                ((x)*(x))

#define Limit(v,l,h)        ((v) > (h)) ? (h) : ((v) < (l)) ? (l) : (v)
#define InLimits(v,l,h)      ((v) > (h)) ? 0 : ((v) < (l)) ? 0 : 1

// does a differ from b by more than D?
#define DiffGreater(a,b,D)        (ABS((a)-(b))>D? : 1 : 0)

// is Z within X and Y?
//  god... you would think it was lisp...
#define WITHIN(X,Y,Z)  ( ( ((Z) >= Min((X),(Y))) & ((Z) <= Max((X),(Y))) ) ? (1):(0) )

#define Min(X,Y) ((X) < (Y) ? (X) : (Y))
#define Max(X,Y) ((X) > (Y) ? (X) : (Y))

#define rad2deg(K) ((K)*(180.0/3.1415926535))
#define deg2rad(K) ((K)*(3.1415926535/180.0))

// ascii nibble to binary
#define ASCNibToBin(A) (((A)>'9')?((A)-'7'):((A)-'0'))
// low binary nibble to ascii
#define BinToASCNib(B) (((B)&0x0F)<9?((B)&0x0F)+'0':((B)&0x0F)+'7')

// convert X,Y to integer offset, I is XMAX J is YMAX
#define xy2i(X,Y,I,J)  (((I)*(Y))+(X))

void chomp(char * string);

int chricmp(char a, char b);
char othercase(char a) ;
char * strupper(char *a) ;
char * strlower(char *a);

#endif

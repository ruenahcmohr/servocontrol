/*

lets start this as a primitive box drawing system



*/
#include "stdio.h"
#include "malloc.h"
#include "danStuff.h"
#include "colours.h"



void LineX(int x1, int y, int x2, char brush) {
  char * tmp;
  int size;
  
  if (x1 == x2 ) // this will be a bug later.
    return;
   
  size = abs( x1 - x2)+1;
    
  tmp = malloc(size+2); 
  memset(tmp, brush, size);
  tmp[size] = '\0';
  
  gotoxy(y, Min(x1, x2));
  printf("%s", tmp);
  free(tmp);
  return;
   
}

void LineY(int x, int y1, int y2, char brush) {
  int y;
   
  if (y1 == y2)
    return;

  for( y = Min(y1, y2); y < Max(y1, y2); y++) {
    gotoxy(y, x); // dos was stupid.
    printf("%c", brush);  
  }
  
  return;

}

void Box(int x1, int y1, int x2, int y2, char brush) {
  LineX(x1, y1, x2, brush);
  LineX(x1, y2, x2, brush);
  LineY(x1, y1, y2, brush);
  LineY(x2, y1, y2, brush); 
  return;
}

void FillBox(int x1, int y1, int x2, int y2, char brush) {
  int y;
  
  if (y1 == y2)
    return;

  for( y = Min(y1, y2); y < Max(y1, y2); y++) {    
    LineX(x1, y, x2, brush);  
  }

  return;
}

/*
int main(void) {

  clrscr();
  textcolor(4);
  //LineX(2, 2, 30, '+');  
  //LineY(2, 2, 10, '+');
  
  FillBox(5, 5, 40, 9, '-');
  Box(5, 5, 40, 9, '*');
  

  
  textcolor(0);
  gotoxy(25,0);

  return 0;
}

*/

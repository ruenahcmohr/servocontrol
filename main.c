/******************************************************

Servo controller for buddy

linux version.


******************************************************/

#include <stdio.h>
#include <string.h>
#include "ssls2.h"
#include "colours.h"
#include "asciishape.h"

#define FALSE           0
#define TRUE            1


#define LISTEN          0
#define IGNORE          1
#define LISTENONCE      2
#define SETFLAGS        3

  #define EXECPOS         0
  #define POWERUP         1
  #define POWERDOWN       2
  #define PRESET          3 

#define SETPOSITION     4
#define CACHEPOS        5
#define GETCURRENT      6
#define GETPOSITION     7
#define GETMODEL        8


void mainMenu(void) ;
void menu1(void) ;
void menu2(void) ;
void menu3(void) ;
void menu6(void) ;
void menu7(void) ;
void menu8(void) ;
void menu10(void);
void menu12(void);
void autodetect();
void currents();
void positions();
void sendCommand(unsigned int command, unsigned int argument);
void cls(void);
void closeserial() ;
void dialog(char *);

int    done;
SSLS_t serial;

unsigned char lastServo;

void closeserial(void) {
  return;
}


int main(void ){  
  
  int Error;
  int c;

  lastServo = 255;
  done = FALSE;
  SerInit(&serial);
  if (SerOpen(&serial, "/dev/ttyS1", 9600) != OK) {
    printf("ERROR OPENING SERIAL\n");
    return 1; 
 }

 // SerSetBaud(&serial, 9600);

  do {
      mainMenu();
  } while ( !done );

  SerFini(&serial);

}



void mainMenu(void) {

  int choice;

  choice = -1;
 while(choice != 99)  {
  choice = -1;
  while ((choice < 1) || (choice > 99)) {
    cls();
    printf("\n\
  1) One time listen and chain address\n\
  2) Call servo to attention\n\
  3) Dismiss servo\n\
  4) Power up\n\
  5) Power down\n\
  6) Set position\n\
  7) Get position\n\
  8) Get device model\n\
  9) Goto cached position\n\
 10) Set cached position\n\
 11) Preset to current position\n\
 12) Get current\n\
 13) Autodetect servos \n\
 14) Position list \n\
 15) Current list \n\
 99) Exit.\n\
 \n");
 textcolor(1);
 printf("  Selection > ");
 textcolor(7); 
  if (scanf("%d", &choice) == EOF) {
    choice = 99;
  }
  textcolor(2);
  }
  
  switch (choice) {
    case 1:  // one time listen
      menu1();
    break;
    
    case 2:  // call servo to attention
      menu2();
    break;
    
    case 3: // dismiss servo
      menu3();
    break;
    
    case 4: // power up
      sendCommand(SETFLAGS, 0x01 << POWERUP);
    break;
    
    case 5: // power down
      sendCommand(SETFLAGS, 0x01 << POWERDOWN);
    break;
    
    case 6: // set position
      menu6();
    break;

    case 7: // get position
      menu7(); // get address
    break;

    case 8: // get device model
      menu8(); // get address
    break;

    case 9: // goto cached position
     sendCommand(SETFLAGS, 0x01 << EXECPOS); 
    break;

    case 10: // set cached position
      menu10(); // get position
    break;

    case 11: // preset to current position
      sendCommand(SETFLAGS, 0x01 << PRESET);
    break;

    case 12: // get current
      menu12(); // get address
    break;

    case 13:
      autodetect();
    break;

    case 14:
     positions();
    break;

    case 15:
     currents();
    break;

    case 99: // quit
      done=1;
    break;
  }

 }


}


void menu1(void) { // one time listen
   int argument;

  argument = -1;
  while ((argument < 0) || (argument > 768)){
   dialog("Which servo would you like to call on? (256 for all) (add 512 for chain select) ");
   scanf("%d", &argument);
  }
  sendCommand(LISTENONCE, argument);
  return;

}

void menu2(void) { // call servo to attention

  int argument;

  argument = -1;
  while ((argument < 0) || (argument > 256)){
   
   dialog("Which servo would you like to call on? (256 for all)  ");
   scanf("%d", &argument);
  }
  sendCommand(LISTEN, argument);
  return;
}


void menu3(void) { // dismiss servo

  int argument;

  argument = -1;
  while ((argument < 0) || (argument > 256)) {
   dialog("Which servo would you like to call off? (256 for all)  ");
   scanf("%d", &argument);
  }
  sendCommand(IGNORE, argument);
  return;
}

// 4 is power up

// 5 is power down

void menu6(void) { // set servo position

  int argument;

  argument = -1;
  while ((argument < 0) || (argument > 1025)){
   dialog("What position? (0 to 1023) ");
   scanf("%d", &argument);
  }
  sendCommand(SETPOSITION, argument);
  return;
}

void menu7(void) { // get position

  int command = GETPOSITION;
  int argument;
  char reply[512];

  argument = -1;
  while ((argument < 0) || (argument > 768)) {
   dialog("Which servo would you like to get position? ");
   scanf("%d", &argument);
  }
  
  if (query(&command, &argument) != 0) {
    sprintf(reply, "Servo reports paramiter %d with value %d", command, argument);
    dialog(reply);
  } else {
    dialog("servo didn't answer");
  }

  getchar();  getchar();

  return;
}

void menu8(void) { // get model
  int command = GETMODEL; 
  int argument;
  char reply[512];

  argument = -1;
  while ((argument < 0) || (argument > 768)) {
   dialog("Which servo would you like to get model?  ");
   scanf("%d", &argument);
  }

  if (query(&command, &argument) != 0) {
    sprintf(reply, "Servo reports paramiter %d with value %d", command, argument);
    dialog(reply);
  } else {
    dialog("servo didn't answer");
  }

  getchar();  getchar();
  return;
}

void menu10(void) { // set cached position
  int argument;

  argument = -1;
  while ((argument < 0) || (argument > 1025)){
   dialog("What position? (0 to 1023) ");
   scanf("%d", &argument);
  }
  sendCommand(CACHEPOS, argument);
 
 return;
}

void menu12(void) { // get current

  int command = GETCURRENT;
  int argument;
  char reply[512];

  argument = -1;
  while ((argument < 0) || (argument > 768)) {
   dialog("Which servo would you like to get current? ");
   scanf("%d", &argument);
  }

  if (query(&command, &argument) != 0) {
    sprintf(reply, "Servo reports paramiter %d with value %d", command, argument);
    dialog(reply);
  } else {
    dialog("servo didn't answer");
  }

  getchar();  getchar();
  return;

}




void sendCommand(unsigned int command, unsigned int argument){

   unsigned char data[2];

   data[0] = (command << 3) | (argument & 0x07);
   data[1] = (argument >> 3 ) | 0x80;

   SerWrite(&serial, data, 2);

}

int query(unsigned int *command, unsigned int *argument) {
   char *data;
   int count = 2;
   

   data = malloc(2);

   data[0] = (*command << 3) | (*argument & 0x07);
   data[1] = (*argument >> 3 ) | 0x80;

   exchange(&serial, &data, &count, 12000);

   if (count == 2) {
    if (((data[0] & 0x80) == 0) && ((data[1] & 0x80) != 0)) {
      *command  = (data[0] >> 3);
      *argument = (*argument & 0xFFF8) | (data[0] & 0x07); // glue in its 0 through 2
      *argument = (*argument & 0x0007) | ((data[1] & 0x7F) << 3); //glue in bits 3 through 9
    }
    return 1;
  } 
  return 0;
}

void autodetect() {
   int command, argument;
   int i;

   cls();
   
   for(i = 0; i < 256; i++) {
     command = GETMODEL;
     argument = i;
     if (query(&command, &argument) != 0) {
        printf("  Servo %03d: version: %d\n", i, argument);
        lastServo = i+1;
     }
   }
   printf(" ------| End of list |---%03d---\n\n", lastServo);
   getchar();  getchar();
   return;
  

}

void positions() {
   int command, argument;
   int i;

   cls();

   for(i = 0; i < lastServo; i++) {
     command = GETPOSITION;
     argument = i;
     if (query(&command, &argument) != 0) {
        printf("  Servo %03d: position: %d\n", i, argument);
     }
   }
   printf(" ------| End of list |------\n\n");
   getchar();  getchar();
   return;


}


void currents() {
   int command, argument;
   int i;

   cls();

   for (i = 0; i < lastServo ; i++) {
      command = GETCURRENT;
      argument = i;
      if (query(&command, &argument) != 0) {
         printf("  Servo %03d: current: %d\n", i, argument);
      }
   }
   printf("---------| End of list |-------\n\n");
   getchar(); getchar();
   return;

}

void dialog(char * string) {

  int start = 21;

  FillBox(2, start, 10+strlen(string), start+4, ' ');  
  Box(2, start, 10+strlen(string), start+4, '?');

  gotoxy(start+2,4);
  printf("%s", string);
  fflush(stdout);
}

void cls(void) {
  clrscr();
  gotoxy(1,1);
  textcolor(4); 
  Box(17, 1, 36, 3, '+');
  gotoxy(2, 18);
  printf(" Servo controller ");
  textcolor(2);
  gotoxy(4, 1);
}

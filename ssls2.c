/* 
   by rue_mohr 2006-2012
   
   - blocking writes
   - default 9600 buad
   
*/

#include "ssls2.h"

// use to send to it
Status_t SerWrite(SSLS_t *this, char *data, int count){
   if (write(this->fd, data, count)< 0) {
      fputs("write failed!\n", stderr);
      return Forbidden;
   }
   
   tcdrain(this->fd); // finish write

   return OK;
}

// use to get 0 or more bytes that are ready from it now
Status_t SerRead(SSLS_t *this, char **data, int * count){

   int bytes, retbytes;
   
   if (*data) { 
      free(*data); // free old buffers
      *data = NULL;
   }
   if (count) *count = 0;   // preset byte count;
   
   ioctl(this->fd, FIONREAD, &bytes);
   if (bytes == 0) return OK;
   
   if ((*data = malloc(bytes+1)) == NULL) {
     *data = NULL;
     return ErrMalloc;
   }
   
   retbytes = read(this->fd, *data, bytes);
 //  printf(">>> %s\n", *data);
   if (count) *count = retbytes;   
     
   if (retbytes < 0) {
      if (errno == EAGAIN) {
         printf("Access problem with serial port\n");
         return Forbidden;
      } else {
         printf("SERIAL read error %d %s\n", errno, strerror(errno));
         return Forbidden;
      }
   } else {
     (*data)[retbytes] = 0;   // use the extra byte for null termination
   }
   
                  
   return OK;
}

// polling mechanism
// use to know how much information is has for you
int SerBytesIn(SSLS_t *this) {
   int bytes;
   ioctl(this->fd, FIONREAD, &bytes);
   return bytes; 
}

//callback mechanism
// use when you want to hear what it says without knowing when it may speak
Status_t SerStartNotifier(SSLS_t *this, void (*charHandler)(char*)) {
   
   // set callback fn.
   this->charHandler = charHandler;
   
   // start thread
   if( (pthread_create( &(this->monitorThread), NULL, (void * (*)(void *))&SerNotifierThread, this)) ){
      printf("Failed to start serial monitor thread \n");
   }
}

// this isn't for you, dont use it
void *SerNotifierThread(SSLS_t *this) {
   fd_set rfds;
   struct timeval tv;
   int retval;

   while(1) {
      FD_ZERO(&rfds);
      FD_SET(this->fd, &rfds);
      FD_SET(0, &rfds);
      tv.tv_sec  = 0 ;
      tv.tv_usec = 250000;
      retval = select((this->fd)+1, &rfds, NULL, NULL, &tv);

      
      if (retval == -1) {
            perror("select()");
      } else if (retval) {              
         if ( FD_ISSET(this->fd, &rfds) ) {      
               
          // read input
          /*
             if ((*data = malloc(bytes+1)) == NULL) {
     *data = NULL;
     return ErrMalloc;
   }
   
   bytes = read(this->fd, *data, bytes);
   if (count) *count = bytes;   
     
   if (bytes < 0) {
      if (errno == EAGAIN) {
         printf("Access problem with serial port\n");
         return Forbidden;
      } else {
         printf("SERIAL read error %d %s\n", errno, strerror(errno));
         return Forbidden;
      }
   } else {
     (*data)[bytes] = 0;   // use the extra byte for null termination
   }
          */
          // call callback with data
            // charHandler(
         }  
      }                       
   }
} 


// use to set communication rate
Status_t SerSetBaud(SSLS_t *this, int baud) {

   struct serial_struct ser_info;
 
   if (baud == 0) return BadArg;
   this->baud = baud;   
   if (this->fd == -1) return NotReady;
   
   ioctl(this->fd, TIOCGSERIAL, &ser_info);                         // change up baud rate
   ser_info.flags        &= ~ASYNC_SPD_MASK;                        // by turning off important things,
   ser_info.flags         |= ASYNC_SPD_CUST ;                       // by turning on the dangerous switches,
   ser_info.custom_divisor = ser_info.baud_base / baud;             // calculating dangerous numbers,
   ioctl(this->fd, TIOCSSERIAL, &ser_info);                         // and maliciously jamming them back into the system.         
      
   return OK;
}

// use to find out current communication rate
Status_t SerGetBaud(SSLS_t *this, int * baud) {

   struct serial_struct ser_info;
 
   ioctl(this->fd, TIOCGSERIAL, &ser_info);
     
   *baud = ser_info.baud_base / ser_info.custom_divisor;  
   
   return OK;
}

// use to initialize data structure
Status_t SerInit (SSLS_t *this){

  this->fd   = -1;
  this->baud = 9600;

  return OK;
}

// use to initiate communications
Status_t SerOpen (SSLS_t *this, char *file, int baud){

   struct termios options;
     
   /*
   O_NDELAY causes non-blocking read/write   
   <DagoRed>  O_FSYNC         All writes immediately effective, no buffering
   <DagoRed> O_EXLOCK        Get an "exclusive lock" on the file 
   but none of these options do squat
   */
   if ((this->fd = open(file, O_RDWR | O_NOCTTY /*| O_SYNC | O_NDELAY*/)) == -1)   return CantOpen;   
                  
   /* this is supposed to set blocking mode, dosn't do squat    
    fcntl(this->fd, F_SETFL, 0);   */
  
   bzero(&options, sizeof(options)); /* clear struct for new port settings */
   
   // Enable the receiver and set local mode...
   /*  options.c_lflag |= ~(ICANON | ECHO | ECHOE);  some useless options you might consider */
   
   options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;    // B38400 allows us to use a custom baud rate                                                       
   options.c_iflag = IGNPAR | IGNBRK ;
          
   // Set the new options for the port...
   
   tcflush(this->fd, TCIFLUSH);
   tcsetattr(this->fd, TCSANOW, &options);  
        
  // SerSetBaud(this, baud);
   
   return OK;
}


/*
NO, I'm not gonna be gracefull and put the port back like it was before we started,
it took a LOT OF WORK to mess it up that much!
*/
// use to clean up when done
Status_t SerFini    (SSLS_t *this) {
  if (!this) return BadArg;
  close(this->fd);
  this->fd = -1;
  return OK;
}

/*

 use timeout to 
  9600 is 104 us per character
  
*/
// use when you know a command will have a more-or-less immediate reply
Status_t converse(SSLS_t * this, char **string, int uswait) {

   int retval; 
   int bytes;

   if ((retval = SerWrite(this, *string, strlen(*string))) != OK) 
       return retval;
                 
   do {
     bytes = SerBytesIn(this);
     usleep(uswait);     
   }  while(SerBytesIn(this) > bytes);         
       
   return SerRead(this, string, NULL);
   
}


// use when you know a command will have a more-or-less immediate reply
// this one is for binary data
Status_t exchange(SSLS_t * this, char **data, int *count, int uswait) {

   int retval; 
   int bytes;

   if ((retval = SerWrite(this, *data, *count)) != OK) 
       return retval;
                 
   do {   // rake up the data
     bytes = SerBytesIn(this);
     usleep(uswait);     
   }  while(SerBytesIn(this) > bytes);         
       
   return SerRead(this, data, count);
   
}

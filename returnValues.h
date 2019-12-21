#ifndef __returnValues
#define __returnValues

typedef enum Status_e { OK = 0, ErrMalloc, Full, BadArg, CantOpen, BadData, EarlyEOF, Overflow, NoMatch, Forbidden, ExecFail, NotReady} Status_t;
//char * status_s[] = { "OK", "Malloc Error", "Full", "Bad Argument","Can't open", "Bad data", "Early EOF", "Overflow", "No match", "Forbidden", "Failed to start" };
extern char * status_s[];

#endif

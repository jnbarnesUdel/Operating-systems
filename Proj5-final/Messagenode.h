#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
 
typedef struct Messagenode
{
  char *message;     // copy of the message 
  int  len;          // length of the message 
  int  sender;       // TID of sender thread 
  int  receiver;     // TID of receiver thread 
  struct Messagenode *next; // pointer to next node 
}messageNode; 

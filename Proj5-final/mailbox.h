#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include "Messagenode.h"
#include "sem_t.h"

typedef struct mailbox
{
  messageNode  *msgfirst;       // message queue start
  messageNode  *msglast;		  // message queue end
 sem_t               *mbox_sem;
}mbox; 

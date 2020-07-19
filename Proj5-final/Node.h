#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
 
typedef struct node
{
  int thread_id;
  int thread_priority;
  ucontext_t *data;
  struct node *next;
}node_t; 

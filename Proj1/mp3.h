#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
typedef struct node
{
  char *name;
  char *song;
  int date;
  int runtime;    
  
  struct node *next;
  struct node *prev;
} node_t; 

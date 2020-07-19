#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include "Node.h"
 
 typedef struct sem
 {
    int count;
    node_t* q;
	node_t* head;
	node_t* end;
 }sem_t;
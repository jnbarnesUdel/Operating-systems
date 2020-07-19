#include "t_lib.h"
#include "Node.h"

ucontext_t *running;
ucontext_t *ready;
node_t *headRun;
node_t *headReady;
node_t *endReady;
int count = 0;

void t_yield()
{
	node_t *tmpNode;
    if(headReady != NULL){	//is there anything to yield to?
		tmpNode = headRun;
		headRun = headReady;
		endReady->next = tmpNode;
		endReady = tmpNode;
		endReady->next = NULL;
		headReady = headReady->next;
		headRun->next = NULL;
		
		running = ready;
		ready = headReady->data;
		
		swapcontext(tmpNode->data, headRun->data);	//currently 
	}  
}

void t_terminate()
{
	node_t *temp;
	temp = headRun;
	headRun = headReady;
	//free(temp->data->uc_stack.ss_sp);	//free ucontext first
	free(temp->data);	//free ucontext first
	free(temp);
	
	running = ready;
	//ready = headReady->next->data;
	headReady = headReady->next;
	headRun->next = NULL;
	setcontext(headRun->data);
}

void t_init()
{
  ucontext_t *tmp;
  node_t *Run = (node_t *) malloc(sizeof(node_t));	//these may need to be above so shutdown can free them
  node_t *Ready = (node_t *) malloc(sizeof(node_t));
  Ready = NULL;	//
  headRun = Run;
  //endReady = Run;
  tmp = (ucontext_t *) malloc(sizeof(ucontext_t));

  getcontext(tmp);    /* let tmp be the context of main() */
  running = tmp;
  Run->data = tmp;
  Run->next = NULL;
}

void t_shutdown(){
	node_t *temp;
	
	while(headReady != NULL){
		temp = headReady;
		headReady = headReady->next;
		free(temp->data);
		free(temp);
	}
	if(headRun->data != NULL){
		free(headRun->data);
		free(headRun);
	}
}

int t_create(void (*fct)(int), int id, int pri)
{
  size_t sz = 0x10000;

  ucontext_t *uc;
  uc = (ucontext_t *) malloc(sizeof(ucontext_t));

  getcontext(uc);
/***
  uc->uc_stack.ss_sp = mmap(0, sz,
       PROT_READ | PROT_WRITE | PROT_EXEC,
       MAP_PRIVATE | MAP_ANON, -1, 0);
***/
  uc->uc_stack.ss_sp = malloc(sz);  /* new statement */
  uc->uc_stack.ss_size = sz;
  uc->uc_stack.ss_flags = 0;
  uc->uc_link = running; //was running
  makecontext(uc, (void (*)(void)) fct, 1, id);
  //ready = uc;
  if(ready == NULL){	//is ready empty
	ready = uc;
	node_t *newNode = (node_t *) malloc(sizeof(node_t));
	headRun->data->uc_link = uc;
	newNode->thread_id = id;
	newNode->thread_priority = pri;
	newNode->data = uc;
	endReady = newNode;
	endReady->next = NULL;
	headReady = newNode;
  }
  else{	//otherwise add to end
	node_t *newNode = (node_t *) malloc(sizeof(node_t));
	newNode->data = uc;
	endReady->next = newNode;
	endReady->data->uc_link = newNode->data;////////////
	newNode->thread_id = id;
	newNode->thread_priority = pri;
	endReady = newNode;
	endReady->next = NULL;
  }
  
  
}

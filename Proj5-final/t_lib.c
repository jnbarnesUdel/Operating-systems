#include "t_lib.h"
#include "mailbox.h"


ucontext_t *running;
ucontext_t *ready;
node_t *headRun;
node_t *headReady;
node_t *endReady;
//node_t *headBlocking;	//you can have multiple semaphores with multiple blocking ques so don't do like this
//node_t *endBlocking;
int count = 0;
mbox *m;
//int waiting = 0; //may need a waiting count for each No just gonna use negative sp->count





int mbox_create(mbox **mb){
	mbox* mail = (mbox*) malloc(sizeof(mbox));
	mail->msgfirst = NULL;
	mail->msglast = NULL;
	m = mail;
	*mb = mail;
}


void mbox_destroy(mbox **mb){
	if(*mb != NULL){
		free(*mb);
	}
}

void mbox_deposit(mbox *mb, char *msg, int len){
	messageNode* temp = (messageNode*) malloc(sizeof(messageNode));
	temp->message = (char*) malloc(len +1);
	temp->message = msg;
	temp->len = len;
	
	if(mb->msgfirst == NULL){ //init
		mb->msgfirst = temp;
		mb->msglast = temp;
		mb->msglast->next = NULL;
	}
	else{ //end of mbox
		mb->msglast->next = temp;
		mb->msglast = temp;
		mb->msglast->next = NULL;
	}
}

void mbox_withdraw(mbox *mb, char *msg, int *len){
	if(mb->msgfirst != NULL){
		msg = mb->msgfirst->message;
		*len = mb->msgfirst->len; 
		
		messageNode *temp;
		temp = mb->msgfirst;
		mb->msgfirst = mb->msgfirst->next;
		free(temp); //can comment this out if need
	}
	else{
		len = 0;
	}
}

void send(int tid, char *msg, int len){
	messageNode *temp = (messageNode*) malloc(sizeof(messageNode));
	temp->sender = headRun->thread_id;
	temp->receiver = tid;
	temp->message = msg;
	temp->len = len;
	if(m == NULL){
		mbox* t = (mbox*) malloc(sizeof(mbox));
		t->msgfirst = NULL;
		t->msglast = NULL;
		m = t;
	}
	if(m->msgfirst == NULL){ //first in mailbox
		m->msglast = temp;
		m->msgfirst = temp;
	}
	else{ //end of mailbox
		m->msglast->next = temp;
		m->msglast = temp;
		m->msglast->next = NULL;
	}
}

void receive(int *tid, char *msg, int *len){
	messageNode* temp;
	messageNode* prev = NULL;
	temp = m->msgfirst;
	while(temp != NULL){
		if(temp->receiver == headRun->thread_id || temp->receiver == 0){	//is it youre thread or any thread
			*tid = temp->sender;
			msg = temp->message;
			*len = temp->len;
			//now remove the message you recieved
			if(prev != NULL){ //temp in middle of list
				if(temp == m->msglast){ //temp at end of list
					m->msglast = prev;
				}
				prev->next = temp->next;
				free(temp);
			}
			else{ //temp at front of list
				m->msgfirst = temp->next;
				free(temp);
			}
			break;
		}
		prev = temp;
		temp = temp->next;
	}
}

int sem_init(sem_t **sp, int sem_count){
	sem_t* s = (sem_t*)(malloc(sizeof(sem_t)));
	//node_t* q = (node_t*)malloc(sizeof(node_t)); //I think i need to malloc this but idk
	s->count = sem_count;
	//s->q = q;   //what do i make q? mabe the blocking que?
	s->head = NULL;
	s->end = NULL;
	*sp = s;	//don't know about this
	return 1;
}

void sem_wait(sem_t *sp){
	if(sp->count <= 0){
		node_t *temp;
		temp = headRun;
		if(sp->count ==0){	//if none in waiting need to initialize blocking que
			//sp->q = temp;
			sp->end = temp;
			sp->head = temp;
			temp->next = NULL;
			//endBlocking->next = NULL;
		}
		else{
			sp->end->next = temp;
			sp->end = temp;
			sp->end->next = NULL;
		}
		
		sp->count = sp->count -1;
		
		if(headReady != NULL){
			headRun = headReady;	//yield and bring next ready to running after putting on blocking list
			headReady = headReady->next;
			headRun->next = NULL;
		}
		running = ready;
		if(headReady != NULL){
			ready = headReady->data;
		}

		swapcontext(temp->data, headRun->data);
		
	}
	else{
		sp->count = sp->count -1;	//decrement the count
	}
}

void sem_signal(sem_t *sp){	//im just considering this to be like post;
	if(sp->head != NULL){	//is there something on the block que
		endReady->next = sp->head;
		endReady = sp->head;
		sp->head = sp->head->next;
	}
	sp->count = sp->count +1;
	
	//t_yield();	//do i yield?
}

void sem_destroy(sem_t **sp){
	if(*sp != NULL){
		//free(*sp->head);
		//free(*sp->end);
		free(*sp);
	}
}
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
	
	if(headReady != NULL){
		running = ready;
		//ready = headReady->next->data;
		headReady = headReady->next;
		headRun->next = NULL;
		setcontext(headRun->data);
	}
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
	if(headRun!= NULL){
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

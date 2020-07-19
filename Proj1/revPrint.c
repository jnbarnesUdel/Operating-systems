#include "mp3.h"

extern node_t *tail;

void revPrint()
{
  node_t *temp;
  int  i = 0;
	//don't know if there is an easy way to find the end of list
  temp = tail;

  while (temp != NULL) {
    printf("(%d)--%s--%s--%d--%d--\n", ++i, temp->name, temp->song, temp->date, temp->runtime);
    temp = temp->prev;
  }
}


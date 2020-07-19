#include "mp3.h"

extern node_t *head;

void print()
{
  node_t *temp;
  int  i = 0;

  temp = head;

  while (temp != NULL) {
    printf("(%d)--%s--%s--%d--%d--\n", ++i, temp->name, temp->song, temp->date, temp->runtime);
    temp = temp->next;
  }
}

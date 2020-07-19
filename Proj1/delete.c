#include "mp3.h"

extern node_t *head;
extern node_t *tail;

void delete(char *name)
{
	node_t *temp, *temp2;
	
	temp = head;
	while(temp != NULL){
		if(strcmp(temp->name, name) == 0){
			if (temp == head) //for if first match
			{
				temp->next->prev = NULL;
				head = temp->next;
				free(temp->name); //free name so it doesn't get lost
				free(temp->song); //free song so it doesn't get lost
				free(temp);  //free rest
				temp = head; //move up temp
			}
			else if(temp == tail){ //for if last match
				temp->prev->next = NULL;
				tail = temp->prev;
				free(temp->name);
				free(temp->song);
				free(temp);
				temp = tail;		
			}
			else{ //for if middle match
				temp2 = temp->next;
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				temp->prev = NULL;
				temp->next = NULL;
				free(temp->name);
				free(temp->song);
				free(temp);
				temp = temp2;			
			}
		}
		else{
			temp = temp->next; //advance
		}
	}
}


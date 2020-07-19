//
// adapted from http://www.cprogramming.com/snippets/source-code/singly-linked-list-insert-remove-add-count
//

#include "mp3.h"
#define  BUFFERSIZE 128

node_t *head;
node_t *tail;

void insert(char *name, char *song, int date, int runtime);
void print();
void revPrint();
void delete(char *name);
void freeList();

int main()
{
  int i, date, time, len;
  struct node *n;
  char buffer[BUFFERSIZE], c, songBuff[BUFFERSIZE];

  head = NULL;
  tail = NULL;

  while (1) {
    printf("\nList Operations\n");
    printf("===============\n");
    printf("(1) Insert\n");
    printf("(2) Delete\n");
    printf("(3) Print Begin to End\n");
    printf("(4) Print End to Begin\n");
    printf("(5) Exit\n");
    printf("Enter your choice : ");
    if (scanf("%d%c", &i, &c) <= 0) {          // use c to capture \n
        printf("Enter only an integer...\n");
        exit(0);
    } else {
        switch(i)
        {
        case 1: printf("Enter the name of artist to insert : ");
		if (fgets(buffer, BUFFERSIZE , stdin) != NULL) {
                  len = strlen(buffer);
                  buffer[len - 1] = '\0';   // override \n to become \0
                } else {
                    printf("wrong name...");
                    exit(-1);
                  }
		
		//Enter song title here
		printf("Enter the Song title to insert : ");
		if (fgets(songBuff, BUFFERSIZE , stdin) != NULL) {
                  len = strlen(songBuff);
                  songBuff[len - 1] = '\0';   // override \n to become \0
                } else {
                    printf("wrong song...");
                    exit(-1);
                  }
		
		//Enter Date here
		printf("Enter the Date as an int to insert :");
		scanf("%d%c", &date, &c);

                printf("Enter the Runtime to insert : ");
                scanf("%d%c", &time, &c);  // use c to capture \n
                printf("[%s] [%s] [%d] [%d]\n", buffer, songBuff, date, time);
                insert(buffer, songBuff, date, time);
                break;
        case 3: if (head == NULL)
                  printf("List is Empty\n");
                else
                  print();
                break;
        case 2: printf("Name the artist\n");
		fgets(buffer, BUFFERSIZE, stdin);
		len = strlen(buffer);
		buffer[len -1] = '\0';
		delete(buffer);
                break;
	case 4: if(head == NULL)
		  printf("List is Empty\n");
		else
		  revPrint();
		break;
        case 5: freeList();
                return 0;
        default: printf("Invalid option\n");
        }
    }
  }
  return 0;
}

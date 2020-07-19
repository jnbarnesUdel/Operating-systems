#include "mp3.h"

extern node_t *head;
extern node_t *tail;

void insert(char *name, char *song, int date, int runtime)
{
  node_t *temp, *mp3;

  mp3 = (node_t *) malloc(sizeof(node_t));        // malloc space for MP3
  mp3->name = (char *) malloc(strlen(name) + 1);  // malloc space for name
  strcpy(mp3->name, name);                        // "assign" name via copy
  
  mp3->song = (char *) malloc(strlen(song) + 1);  // give song name and memory alocate
  strcpy(mp3->song, song);  
  
  mp3->date = date;                                // give value to date
  
  mp3->runtime = runtime;			   // give runtime value

  mp3->next = NULL;
  
  mp3->prev = NULL;

  if (head == NULL)
  {
    head = mp3;               // add the first MP3
    tail = mp3;
  }
  else
  {
    temp = tail;
    temp->next = mp3;         // append to the tail/end
    mp3->prev = temp;         // give the previouse
    tail = mp3;		      //reassign tail
  }
}

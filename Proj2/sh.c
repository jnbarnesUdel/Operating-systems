#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"

int sh( int argc, char **argv, char **envp )
{
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *w, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  int pid;
  char *pr = malloc(PROMPTMAX);
  pr[0] = 0;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;
  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start
 						  out with*/
  
	   
  if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
  {
    perror("getcwd");
    exit(2);
  }
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();

  while ( go )
  {
	pwd = getcwd(NULL, PATH_MAX +1);	
	printf("%s [%s]>>", pr, pwd);
	fgets(prompt, PROMPTMAX, stdin);
	if(prompt[strlen(prompt) -1] == '\n'){
		prompt[strlen(prompt)- 1] = 0; //make last line 0
	}
	
	argsct = 0;

	args[argsct] = strtok(prompt, " ");
	argsct++;						//token all the args
	while((args[argsct] = strtok(NULL, " ")) != NULL){
		argsct++;
	}
	
	//direct
	arg = args[1];
	if(strcmp(args[0], "which") ==0){
			printf("Executing Built in %s \n", args[0]);
                        if((commandpath = which(arg, pathlist)) == NULL){
                                printf("Command not found");
                        }
                        else{
                                printf("%s \n", commandpath);
                        }
        }
	else if(strcmp(args[0], "where") ==0){
                        printf("Executing Built in %s \n", args[0]);
                        if((commandpath = where(arg, pathlist)) == NULL){
                                printf("Command not found");
                        }
                        else{
                                printf("%s \n", commandpath);
                        }
        }
	else if(strcmp(args[0], "exit") ==0){
                        printf("Executing Built in %s \n", args[0]);
		break;
	}
	else if(strcmp(args[0], "cd") ==0){
                        printf("Executing Built in %s \n", args[0]);
		cd(arg, pwd);
	}
	else if(strcmp(args[0], "pwd") ==0){
                        printf("Executing Built in %s \n", args[0]);
		printf("%s \n", pwd);
	}
	else if(strcmp(args[0], "list") ==0){
                        printf("Executing Built in %s \n", args[0]);
		list(pwd, args, argsct);
	}
	else if(strcmp(args[0], "pid") ==0){
                        printf("Executing Built in %s \n", args[0]);
		printf("pid is: %ld \n", getpid);
	}
	else if(strcmp(args[0], "kill") ==0){
                        printf("Executing Built in %s \n", args[0]);
		k(args, argsct);
	}
	else if(strcmp(args[0], "prompt") == 0){
                        printf("Executing Built in %s \n", args[0]);
		if(argsct == 2){
			strcpy(pr, args[1]);
		}
		else{
			printf("Enter a prompt: ");
			fgets(pr, PROMPTMAX, stdin);
			pr[strlen(pr)-1] = 0; 
		}
	}
	else if(strcmp(args[0], "printenv")== 0){
                        printf("Executing Built in %s \n", args[0]);
		if(argsct > 2){
			printf("Too many arguments \n");
		}
		else if(argsct == 2){
			printf("%s \n", getenv(args[1]));
		}
		else{
			while(*envp){
				printf("%s \n", *envp);
				envp++;	
			}	
		}
	}
	else if(strcmp(args[0], "setenv")==0){
                        printf("Executing Built in %s \n", args[0]);
		sete(args, argsct, envp);
	}
	else{ //look up
		char* c = malloc(125);
		int acc = 1;
		if(access(args[0], X_OK) ==0){
			strcpy(c, args[0]);
		}
		else{
			if((c = which(args[0], pathlist)) == NULL){
				printf("command %s does not exist \n", args[0]);
				acc = 0;
			}
		}
		if(acc == 1){
			printf("Executing %s \n", c);
			pid = fork();
			if((pid  < 0)){
				printf("fork err");
				exit(1);
			}
			else if(pid == 0){ //child
				execve(c, &args[0], NULL);
				printf("execve err");
				break;			
			}
			else{	//parent
				waitpid(pid, NULL, 0);
				printf("parrent done \n");
			}
		}
		free(c);
	}
  }
  free(prompt);
  free(commandline);
  free(command);
  free(args);
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
	char *w = malloc(sizeof(command) + sizeof(pathlist->element) + 10); //store name of thing and path to check access
  	while (pathlist) {         // WHICH
		sprintf(w, "%s/%s", pathlist->element, command);
    		if (access(w, X_OK) == 0) {  //check access
			strcpy(command, w);
			free(w); //free
      			return command;
    		}
    	pathlist = pathlist->next;
  	}
	free(w);
	return NULL;
	
} /* which() */

char *where(char *command, struct pathelement *pathlist )
{
  /* similarly loop through finding all locations of command */
	char *w = malloc(sizeof(command) + sizeof(pathlist->element) + 10);
	char *list = malloc(120); //just picked a big number for this since it could get big
	//char *temp = malloc(120);
	list[0] = 0;
	while (pathlist) {         // WHERE
		sprintf(w, "%s/%s", pathlist->element, command);
    		if (access(w, X_OK) == 0){
			strcat(list, w);
			//sprintf(list, "%s \n %s", w, temp);  //lsit all posible places with a /n between them
    			//strcpy(temp, list);
		}
	pathlist = pathlist->next;
  }
	strcpy(command, list);
	free(list);
	free(w);
	//free(temp);
	return command;
} /* where() */



void list ( char *dir, char **args, int argsct)
{
	DIR *d;
	struct dirent* dt;
 	if(argsct == 1){
		d = opendir(dir);
		while((dt = readdir(d)) != NULL){
			printf("%s \n", dt->d_name); 
		}
	} 
	else{
		while(argsct != 1){
			argsct = argsct -1;
			printf("%s: \n", args[argsct]);
			if((d = opendir(args[argsct]))==NULL){
				printf("%s does not exist \n", args[argsct]);
				exit(1);
			}
			while((dt = readdir(d)) != NULL){
				printf("%s \n", dt->d_name);
			}
		}	
	}
} /* list() */

void cd(char *arg, char *cwd)
{
	char *h = malloc(120);
	char *start = malloc(2);
	if(arg != 0){ 			
		start[0] = arg[0];
		start[1] = 0;
	}
	if(arg == 0){			//home
		sprintf(h, "%s", getenv("HOME"));
		chdir(h);
	}
	else if(strcmp(arg, "-") == 0){      // back one
		chdir("..");
	}
	else if (strcmp(start, "/") == 0){	//abs path
		chdir(arg); 
	}
	else{					//rel path
		sprintf(h, "%s/%s", cwd, arg);
		chdir(h);		
	}
	free(start);
	free(h);	
}

void k(char **args, int argsct){
	int one;
	int two;
	if(argsct == 2){ // one arg kill
		one = atoi(args[1]);
		kill(one, 15);
	}
	else if(argsct == 3){ //2 arg kill
		args[1][0] =  " ";
		two = atoi(args[1]);
		one = atoi(args[2]);
		kill(one, two);
	}
	else{  //error
		printf("incorrect usage of kill");
	}

}

sete(char **args, int argsct, char **envp){
	if(argsct == 1){   //print whole thing
		while(*envp){
			printf("%s \n", *envp);
			*envp++;
		}
	}
	else if(argsct == 2){
		setenv(args[1], "", 1); //empty for one arg
	}
	else if(argsct == 3){  //set new env with arg 2 as value
		setenv(args[1], args[2], 1);
	}
	else{
		printf("Incorrect usage of setenv");
	}

}

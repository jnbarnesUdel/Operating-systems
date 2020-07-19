
#include "get_path.h"

int pid;
int sh( int argc, char **argv, char **envp);
char *which(char *command, struct pathelement *pathlist);
char *where(char *command, struct pathelement *pathlist);
void list ( char *dir, char **args, int argsct );
void cd(char *arg, char *cwd);
void printenv(char **envp);
void k(char **args, int argsct);
void c(int sig);

#define PROMPTMAX 32
#define MAXARGS 10

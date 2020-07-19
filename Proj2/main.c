#include "sh.h"
#include <signal.h>
#include <stdio.h>

void sig_handler(int signal); 

int main( int argc, char **argv, char **envp )
{
  /* put signal set up stuff here */
 	signal(SIGINT, sig_handler);
        sigignore(SIGTSTP);
        sigignore(SIGTERM);


  return sh(argc, argv, envp);
}

void sig_handler(int signal)
{
  /* define your signal handler */
}


/* PA 1 by Nghi Vu */ 

#include "apue.h"
#include <sys/wait.h>

static void sig_int(int);	/* our signal-catching function */

int
main(void) {
  char buf[MAXLINE];	/* from apue.h */
  pid_t pid;
  int status;
  int num_of_cmd = 0;
  char* token;

  if (signal(SIGINT, sig_int) == SIG_ERR)
    err_sys("signal error");

  printf("%% ");	/* print prompt (printf requires %% to print %) */
  while (fgets(buf, MAXLINE, stdin) != NULL) {

    /* initializes dynam array of strings to 0 with max elements of MAXLINE */
    char **cmd_str = (char **)calloc(MAXLINE, sizeof(char*));
    if (buf[strlen(buf) - 1] == '\n')
      buf[strlen(buf) - 1] = 0; /* replace newline with null */

     /* tokenizes first word up to first space into var "token" */ 
    token = strtok(buf, " ");
    
    /* while buf hasn't been fully tokenized yet && the number of args / options in 
     * buf cmd hasn't exceeded max number of alloweed cmd args (MAXLINE) */
    while (token != NULL && num_of_cmd < MAXLINE) {

      /* allocates memory for each option / arg string in cmd_str */ 
      cmd_str[num_of_cmd] = (char*)calloc(strlen(token) + 1, sizeof(char*));
      
      /* copies token to a string in cmd_str array */ 
      strcpy(cmd_str[num_of_cmd], token);

      /* NULL will use the same source string to tokenize */ 
      token = strtok(NULL, " ");

      printf("cmd_str[%d]: %s\n", num_of_cmd, cmd_str[num_of_cmd]);

      /* inc. the number of cmd options / args included so far */ 
      num_of_cmd++;
    }

    printf("\n");

    if ((pid = fork()) < 0) 
      err_sys("fork error");
    else if (pid == 0) { /* child */
      
      execvp(cmd_str[0],cmd_str);
      err_ret("couldn't execute, %s", buf);
      exit(127);
    }

    /* parent */
    if ((pid = waitpid(pid, &status, 0)) < 0)
      err_sys("waitpid error");
    
    /* frees dynam array cmd_str so user can enter new command */ 
    free(cmd_str);
    num_of_cmd = 0;
    printf("%% ");
  }

  exit(0);
}

void
sig_int(int signo) {
  printf("interrupt\n%% ");
}

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/wait.h> 
#include <stdlib.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

// WARNING: It seems that it doesnt output anything if you try to run it more than once w/o re making the file. If you still have issues lmk

int main(int argc, char **argv)
{
  int pipefd1[2];
  int pipefd2[2];
  pid_t pid1;
  pid_t pid2;
  int i;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL, NULL};

  // make a pipe (fds go in pipefd[0] and pipefd[1])
  pipe(pipefd1);
  pipe(pipefd2);

  pid1 = fork();

  if (pid1 == 0)
    {
    pid2 = fork();
    
      if (pid2 == 0){
       
      // replace standard input with input part of pipe
      dup2(pipefd2[0], 0);

      // close unused hald of pipe
      for (i = 0; i < 2; i++){
        close(pipefd1[i]);
        close(pipefd2[i]);
      }

   
      // execute sort
      execvp(sort_args[0], sort_args);
      }
    
      else {
      
        //replace the input
        dup2(pipefd1[0], 0);
        dup2(pipefd2[1], 1);

       //close all the pipes 
        for (i = 0; i < 2; i++){
          close(pipefd1[i]);
          close(pipefd2[i]);
        }
  
        // execute grep
        execvp(grep_args[0], grep_args);
      }
  } else {
      //replace the input
      dup2(pipefd1[1], 1);
    
    
      //close all the pipes 
       for (i = 0; i < 2; i++){
         close(pipefd1[i]);
         close(pipefd2[i]);
       }
    
      // execute cat
      execvp(cat_args[0], cat_args);
    
  }
  wait(NULL);
  
  return 0;
}

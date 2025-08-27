#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <setjmp.h>
#include <string.h>

void main(){
    pid_t pid2 = vfork(); 
    if (pid2 < 0){
            printf("Can't create child process\n");
    }

    if (pid2 == 0){
        char *argv_for_program[] = { "/opt/run_exec", NULL };
        printf ("Write by child process \n");
        execv("/opt/run_exec", argv_for_program);
    }else{
        printf ("Write by parent process %d \n", pid2);
        sleep(2);
        kill(pid2,SIGFPE);
        while (1);
        printf("%s\n","Proccess1 end");
    }
}

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>

jmp_buf fpe;

void handler(int signum)
{
    // Do stuff here then return to execution below
    printf("in sigfpe handler\n");
    longjmp(fpe, 1);
}

void handler_sigill(int signum)
{
    // Do stuff here then return to execution below
    printf("in sigill handler\n");
    longjmp(fpe, 1);
}

int main()
{
    volatile int i, j;

    struct sigaction act;
    struct sigaction oldact;
    memset(&act, 0, sizeof(act));
    act.sa_handler = handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGFPE, &act, &oldact);


    struct sigaction act1;
    struct sigaction oldact1;
    memset(&act1, 0, sizeof(act1));
    act1.sa_handler = handler_sigill;
    act1.sa_flags = SA_SIGINFO;
    sigaction(SIGILL, &act1, &oldact1);
    printf("Before condition\n");

    if (0 == setjmp(fpe))
    {
        printf("Waiting\n");
          int x = 0;
     //    j = i / 0;
       while(1){
          x++;
          if (x%10000 == 0){
                printf("Waiting\n");
          }


       }
    }
    printf("After for condition\n");

    return 0;
}

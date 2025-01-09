## Scone allows malicious signal injection

### Tested on versions:
Scone versions 5.8.0 and 5.9.0

### Description:
An attacker can break the security of an SGX application that uses Scone Libary OS. Scone assumes an untrusted operating system that can inject signals to the SGX enclave. Scone forwards signals from the operating system to the SGX enclave. A malicious operating system can compromise the confidentiality and integrity of apps running inside Scone by injecting signals and arbitrarily triggering enclave signal handlers.  
See details [here](https://arxiv.org/abs/2404.13998).

### Reported by:
Supraja Sridhara, Andrin Bertschi, Benedict Schlüter, Shweta Shinde

### References: 
SIGY: Breaking Intel SGX Enclaves with Malicious Exceptions & Signals : https://arxiv.org/abs/2404.13998
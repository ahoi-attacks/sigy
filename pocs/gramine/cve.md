## Gramine allows malicious signal injection

### Affected versions:
Gramine Library OS with Intel SGX Support before v1.6

### Description:
An attacker can break the security of an SGX application that uses Gramine Libary OS. Gramine assumes an untrusted operating system that can inject signals to the SGX enclave. Gramine forward signals from the operating system to the SGX enclave. A malicious operating system can compromise the confidentiality and integrity of apps running inside Gramine by injecting signals and arbitrarily triggering enclave signal handlers.  
See details [here](https://arxiv.org/abs/2404.13998).

This vulnerability is patched in [v1.6](https://github.com/gramineproject/gramine/pull/1570).

### Credit: 
Supraja Sridhara, Andrin Bertschi, Benedict Schlüter, Shweta Shinde

### References: 
SIGY: Breaking Intel SGX Enclaves with Malicious Exceptions & Signals : https://arxiv.org/abs/2404.13998
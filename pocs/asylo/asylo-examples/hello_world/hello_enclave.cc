/*
 *
 * Copyright 2018 Asylo authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <cstdint>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>

#include "absl/status/status.h"
#include "absl/strings/str_cat.h"
#include "asylo/trusted_application.h"
#include "asylo/util/logging.h"
#include "asylo/util/status.h"
#include "hello_world/hello.pb.h"
jmp_buf fpe;

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

class HelloApplication : public asylo::TrustedApplication {
 public:
  HelloApplication() : visitor_count_(0) {}


  asylo::Status Run(const asylo::EnclaveInput &input,
                    asylo::EnclaveOutput *output) override {
    if (!input.HasExtension(hello_world::enclave_input_hello)) {
      return absl::InvalidArgumentError(
          "Expected a HelloInput extension on input.");
    }
    std::string visitor =
        input.GetExtension(hello_world::enclave_input_hello).to_greet();

    LOG(INFO) << "Hello " << visitor;
    if (output) {
      LOG(INFO) << "Incrementing visitor count...";
        struct sigaction act;
        struct sigaction oldact;
        memset(&act, 0, sizeof(act));
        act.sa_handler = handler;
        act.sa_flags = SA_SIGINFO;
        sigaction(SIGFPE, &act, &oldact);
        int i, j = 1;
        if (0 == setjmp(fpe))
        {
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
        }
      printf("New mean is : %d\n", new_mean);
      output->MutableExtension(hello_world::enclave_output_hello)
          ->set_greeting_message(
              absl::StrCat("Hello ", visitor, "! You are visitor #",
                           ++visitor_count_, " to this enclave."));
    }
    return absl::OkStatus();
  }

 private:
  uint64_t visitor_count_;
};

namespace asylo {

TrustedApplication *BuildTrustedApplication() { return new HelloApplication; }

}  // namespace asylo

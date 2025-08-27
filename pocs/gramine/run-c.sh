cp c-poc/helloworld.c examples/gcc/test_files 
cp c-poc/Makefile examples/gcc
cd examples/gcc

make SGX=1 check


cp java-poc/MultiThreadMain.java examples/openjdk/

cd examples/openjdk
make clean
make SGX=1

gramine-sgx java -Xmx8G MultiThreadMain

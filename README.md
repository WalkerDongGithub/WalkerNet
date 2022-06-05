# WalkerNet

WalkerNet is an SDN project running on Linux environment.
Walker targets this project as a laboratory to study the network.

This project contain (and will contain):

1. A stable and multiple Physical Layer, such as error-free channel, and error-high channel.
2. A configurable link layer, network layer and even higher.
3. A compiler which can compile a language designed by Walker.
4. A virtual network environment use mininet, with some difference.

## How to run

For convenience, Walker use dependencies as little as possible. 

Now you just need pcap installed, without other dependencies.

And some submodules contain a CMakeLists.txt, every CMakeLists.txt describe a way to compile a whole test.
Therefore, if a submodule contains a CMakeLists.txt, it is probable to reuse as an independent module to other project.

For example, if you want to compile Physical Layer Test.
you just 
```shell
cd PhysicalLayer
cmake .
make 
```
you can build this project.

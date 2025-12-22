# Nearest AP (Linux)

This is the deployment of the Nearest Access Point for linux. This deployment create a simulation, 
mostly for testing purpose, where **n** independent node communicates through a virtual bus 
and find the best leader between them. Also a **console logger** is implemented to speed up debugging.

# Requirements

- [gcc](https://gcc.gnu.org/install/)
- [make](https://www.gnu.org/software/make/)
- [wget](https://www.gnu.org/software/wget/)

# Setup

To setup the environment for development or just building run:
```sh
make setup
```
The command will download the [nanopb](https://github.com/nanopb/nanopb) sources used in the
compilation for  serialization and deserialization of messages.

# Build

> To build you first need to do the [Setup](#Setup)

To build the program:
```sh
make
```
The binary is **main**

# Clean

To clean the build:
```sh
make clean
```

To clean the setup:
```sh
make clean_setup 
```

To clean all:
```sh
make clean_all 
```

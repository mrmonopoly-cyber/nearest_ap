# Nearest AP (crazyfly 2.X)

This is the deployment of the Nearest Access Point for the crazyfly drones


# Requirements

To compile and flash the firmware the following software are required:

## Linux:

- [arm-gcc toolchain](https://gcc.gnu.org/)
- [bear](https://github.com/rizsotto/Bear)
- [python](https://www.python.org/)
- git
- make
- [openocd](https://openocd.org/)

## Others:

Not supported


# Setup

To setup the environment for development or just building run:
```sh
make setup
```
The command will:

- get the basic firmware from the official [repository](https://github.com/bitcraze/crazyflie-firmware.git)
- setup the compilation configuration to **cf2** 
- create a **python virtual environment in .venv**. It is necessary to flash the firmware
- add your user to the **plugudev** group, necessary to flash. **This step requires root permissions**
- setup the udev rules for the radio antenna. **This step requires root permissions**
- setup the clangd lsp by creating:

    - **compile_commands.json** with [bear](https://github.com/rizsotto/Bear)
    - **.clangd** to tell clangd the compiler to use and where to find std libraries


# Build

> To build you first need to do the [Setup](#Setup)

To build the firmware for the drone to the following command:
```sh
make
```

# Flash

> To build you first need to do the [Build](#Build)

To flash the firmware for the drone to the following command:
```sh
make
```
The flash will be done through the crazyfly **antenna**


# Clean 
To clean the setup run:
```sh
make clean_setup
```

# The firmware uses the Kbuild build system. There are 'Kbuild' files in this
# example that outlays what needs to be built. (check src/Kbuild).
#
# The firmware is configured using options in Kconfig files, the
# values of these end up in the .config file in the firmware directory.
#
# By setting the OOT_CONFIG (it is '$(PWD)/oot-config' by default) environment
# variable you can provide a custom configuration. It is important that you
# enable the app-layer. See app-config in this directory for example.

SHELL := /bin/bash
TARGET ?=
UDEV_PATH := /etc/udev/rules.d
UDEV_FILE := 99-bitcraze.rules
CC_INCLUDE_PATH := $(shell echo | arm-none-eabi-gcc -E -Wp,-v - 2>&1 | grep -v ignoring | grep -v "#" | grep -v "End of search list." | uniq)
# CXX_INCLUDE_PATH := $(shell echo | arm-none-eabi-g++ --print-sysroot)
CXX_PATH := $(shell whereis arm-none-eabi-g++ | cut -d' ' -f2)
IN_PLUGDEV := $(shell groups $$USER | grep plugdev)

#
# We want to execute the main Makefile for the firmware project,
# it will handle the build for us.
#
CRAZYFLIE_BASE := lib/crazyflie-firmware/

#
# We override the default OOT_CONFIG here, we could also name our config
# to oot-config and that would be the default.
#
OOT_CONFIG := $(PWD)/app-config

NO_INCLUDE_TARGETS := setup clean_setup remove_plug_udev_group clangd_update_compile_command

ifeq ($(filter $(firstword $(MAKECMDGOALS)),$(NO_INCLUDE_TARGETS)),)
OOT_USES_CXX := 1
include $(CRAZYFLIE_BASE)/tools/make/oot.mk
endif

ifdef IN_PLUGDEV
remove_plug_udev_group:
	echo "removing group plugdev from $$USER"
	sudo gpasswd -d $$USER plugdev
else
remove_plug_udev_group:
endif

clangd_update_compile_command:
	sed -i 's/\"-fno-var-tracking-assignments\",//g' compile_commands.json
	sed -i 's/\"-fconserve-stack\",//g' compile_commands.json
	sed -i 's/\"-mfp16-format=ieee\",//g' compile_commands.json
	sed -i 's/\"-mcpu=cortex-m4\",//g' compile_commands.json
	sed -i 's/\"-mfloat-abi=hard\",//g' compile_commands.json
	sed -i 's/\"-mfpu=fpv4-sp-d16",//g' compile_commands.json

clangd_setup:
	echo "cleaning old setup"
	make clean
	rm -f .clangd compile_commands.json

	@echo generating compile_commands.json
	source .venv/bin/activate && intercept-build make
	make clangd_update_compile_command

clean_setup: remove_plug_udev_group
	rm -rf ./lib/crazyflie-firmware
	rm -rf ./build*
	rm -rf ./.venv
	rm -rf ./.cache
	rm -rf ./.clangd
	rm -rf ./compile_commands.json
	@echo "removing udev rules"
	sudo rm -f $(UDEV_PATH)/$(UDEV_FILE)

setup: $(UDEV_PATH) clean_setup 
	@echo "cloning basic firmware of crazyflie"
	git clone --recursive https://github.com/bitcraze/crazyflie-firmware.git lib/crazyflie-firmware;
	cd ./lib/crazyflie-firmware/ && make  cf2_defconfig

	@echo "adding $$USER to the plugdev group"
	sudo sudo usermod -a -G plugdev $$USER
	@echo "copying udev rules in " $(UDEV_PATH)
	sudo cp $(UDEV_FILE) $(UDEV_PATH)

	@echo "creating python virtual env in .venv"
	python -m venv .venv
	source .venv/bin/activate && pip install --upgrade pip

	@echo "installing cfclient in .venv"
	source .venv/bin/activate && pip install cfclient
	@echo "installing cflib in .venv"
	source .venv/bin/activate && pip install cflib
	@echo "installing in scan-build .venv"
	source .venv/bin/activate && pip install scan-build


	make clangd_setup
	@echo you have to execute the following command: "source ./.venv/bin/activate"

clean_here:
	rm -rf *.o
	rm -rf src/*.o

clean: clean_here

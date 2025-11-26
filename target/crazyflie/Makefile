# The firmware uses the Kbuild build system. There are 'Kbuild' files in this
# example that outlays what needs to be built. (check src/Kbuild).
#
# The firmware is configured using options in Kconfig files, the
# values of these end up in the .config file in the firmware directory.
#
# By setting the OOT_CONFIG (it is '$(PWD)/oot-config' by default) environment
# variable you can provide a custom configuration. It is important that you
# enable the app-layer. See app-config in this directory for example.

TARGET ?=

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

NO_INCLUDE_TARGETS := setup clean_setup

ifeq ($(filter $(firstword $(MAKECMDGOALS)),$(NO_INCLUDE_TARGETS)),)
-include $(CRAZYFLIE_BASE)/tools/make/oot.mk
endif

clangd_setup:
	bear -- make -j$(nproc)

clean_setup:
	rm -rf ./lib/crazyflie-firmware
	rm -rf ./build*
	rm -rf ./.venv
	rm -rf ./.cache

setup: clean_setup
	git clone --recursive https://github.com/bitcraze/crazyflie-firmware.git lib/crazyflie-firmware;
	@cd ./lib/crazyflie-firmware/ && make  cf2_defconfig

	@python -m venv .venv
	@.venv/bin/pip install cfclient
	@.venv/bin/pip install cflib

	make clangd_setup
	@echo you have to execute the following command: "source ./.venv/bin/activate"



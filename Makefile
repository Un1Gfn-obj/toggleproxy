
include $(THEOS)/makefiles/common.mk

CPP:=/bin/false
CXX:=/bin/false
DEBUG:=1
messages:=no

TOOL_NAME = toggleproxy

toggleproxy_FILES = main.c cfd.m
toggleproxy_CFLAGS = -fobjc-arc
toggleproxy_CODESIGN_FLAGS = -Sentitlements.plist
toggleproxy_INSTALL_PATH = /usr/local/bin

include $(THEOS_MAKE_PATH)/tool.mk

purge:
	$(MAKE) clean
	rm -rf packages/ .theos/obj/

# sourced functions not available in sh subshell
# run:
# 	ssh2 rm -fv \~/toggleproxy
# 	scp2 .theos/obj/debug/toggleproxy
# 	ssh2 \~/toggleproxy

ALL_OBJCFLAGS:=-Wall -Wextra -g $(ALL_OBJCFLAGS) -UNDEBUG -UNS_BLOCK_ASSERTIONS
ALL_CFLAGS:=-Wall -Wextra -g $(ALL_CFLAGS) -UNDEBUG

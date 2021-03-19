CPP:=/bin/false
CXX:=/bin/false
DEBUG:=1
# messages:=yes

include $(THEOS)/makefiles/common.mk

TOOL_NAME = toggleproxy

# toggleproxy_FILES += m.m
toggleproxy_FILES += privilege.c
toggleproxy_FILES += c.c
toggleproxy_FILES += main.c

toggleproxy_CFLAGS = -fobjc-arc
toggleproxy_CODESIGN_FLAGS = -Sentitlements.plist
toggleproxy_INSTALL_PATH = /usr/local/bin

include $(THEOS_MAKE_PATH)/tool.mk

ALL_OBJCFLAGS:=-Wall -Wextra -g $(ALL_OBJCFLAGS) -UNDEBUG -UNS_BLOCK_ASSERTIONS # -Wno-error -Wdeprecated-declarations
ALL_CFLAGS:=-Wall -Wextra -g $(ALL_CFLAGS) -UNDEBUG # -Wno-error -Wunused-parameter

purge:
	$(MAKE) clean
	rm -rf packages/ .theos/obj/

# Sourced functions not propogated to subshell unless bash(1) - Compound Commands - (list)
# ios.bashrc: export -f <function>
transfer:
	@ssh2 'sh -c "test $$(whoami) = root"'
	@ssh2 rm -fv /private/var/mobile/$(TOOL_NAME)
	@scp2 .theos/obj/debug/$(TOOL_NAME)
	@ssh2 'sh -c "chown -v --reference=/bin/su /private/var/mobile/$(TOOL_NAME) && chmod -v --reference=/bin/su /private/var/mobile/$(TOOL_NAME)"'
# 	@echo
# 	@sha1sum .theos/obj/debug/$(TOOL_NAME)
# 	@echo

TARGET := iphone:clang:latest:14.4

include $(THEOS)/makefiles/common.mk

TOOL_NAME = toggleproxy

toggleproxy_FILES = main.m
toggleproxy_CFLAGS = -fobjc-arc
toggleproxy_CODESIGN_FLAGS = -Sentitlements.plist
toggleproxy_INSTALL_PATH = /usr/local/bin

include $(THEOS_MAKE_PATH)/tool.mk

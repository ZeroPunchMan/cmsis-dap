
COMMON_SRC = $(wildcard common/clib/src/*.c) \
	$(wildcard common/armv7-m-helper/*.c)

COMMON_INC = -Icommon/clib/inc \
	-Icommon/ \
	-Icommon/armv7-m-helper



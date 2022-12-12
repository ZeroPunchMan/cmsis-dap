
APP_SRC = $(wildcard application/*.c) \
$(wildcard application/daplink/*.c)

APP_INC = -Iapplication \
-Iapplication/daplink

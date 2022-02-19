

ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    RM_CMD = -del /Q
else
    RM_CMD = rm -fR
endif



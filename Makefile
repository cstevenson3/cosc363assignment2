MAKE_COMMAND := 
MAKEFILE_NAME := 

ifeq ($(OS),Windows_NT)
	MAKE_COMMAND += mingw32-make
	MAKEFILE_NAME += MakefileWindows
else
    UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
        MAKE_COMMAND += make
	    MAKEFILE_NAME += MakefileMac
    else
        MAKE_COMMAND += make
	    MAKEFILE_NAME += MakefileUnix
	endif
endif

dummy.recipe:
	$(MAKE_COMMAND) -f $(MAKEFILE_NAME)

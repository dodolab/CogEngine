# Attempt to load a config.make file.
# If none is found, project defaults in config.project.make will be used.
ifneq ($(wildcard config.make),)
	include config.make
endif

ifdef OF_ANDROID
   # make sure the the OF_ROOT location is defined
ifndef OF_ROOT
    OF_ROOT=H:/OF/openframeworks/Android
endif
endif

ifdef OF_WINDOWS
   # make sure the the OF_ROOT location is defined
ifndef OF_ROOT
    OF_ROOT=/H/OF/openframeworks/msys
endif
endif

copydep:
	@echo "Copying all dependencies"
	@mkdir -p bin
	@$(foreach asset,$(PROJECT_ASSETS),cp -a $(asset) bin &&) echo DONE

all:


# call the project makefile!
include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk

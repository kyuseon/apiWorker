################################################################################
#
#
#
################################################################################

export G_SYSTEM=__SYS_BRIDGE__
export G_BIN_PATH=~/bin/bridge

export PROJECT_HOME=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))
# $dir returns path with trailing slash
export PROJECT_LIB=$(PROJECT_HOME)lib
export PROJECT_SHARE=$(PROJECT_HOME)share

# do not export, use CC, CL
GLOBAL_CC_OPT=-std=c++11 -Wall -g
GLOBAL_CC_INC=-I$(PROJECT_LIB) -I$(PROJECT_SHARE)
GLOBAL_CL_OPT=
GLOBAL_CL_LIB=-L$(PROJECT_LIB) -L$(PROJECT_SHARE)

export CC=g++ $(GLOBAL_CC_OPT) $(GLOBAL_CC_INC)
export CL=g++ $(GLOBAL_CL_OPT) $(GLOBAL_CL_LIB)
export RM=@rm -f
export CP=@cp -f
export CD=@cd
export AR=@ar
export MD=@mkdir -p
export RD=@rmdir
export ECHO=@echo
export MAKE=make

all:
	$(CD) lib && $(MAKE)
	$(CD) proc && $(MAKE)
	$(CD) test && $(MAKE)

clean:
	$(CD) lib && $(MAKE) clean
	$(CD) proc && $(MAKE) clean
	$(CD) test && $(MAKE) clean

install:
	$(CD) proc && $(MAKE) install


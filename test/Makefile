################################################################################
#
#
#
################################################################################

all:
	$(CD) application && $(MAKE) && echo "_________    BUILD LIB TEST DONE    _________"

clean:
	$(CD) application && $(MAKE) clean && echo "_________    LIB TEST CLEANED    _________"

install:
	$(CD) application && $(MAKE) install && echo "_________    LIB TEST INSTALLED    _________"

test:
	$(ECHO) $(PROJECT_HOME)
	$(ECHO) "$(MAKEFILE_LIST)"
	$(ECHO) $(lastword $(MAKEFILE_LIST))
	$(ECHO) $(firstword $(MAKEFILE_LIST))
	$(ECHO) $(abspath $(lastword $(MAKEFILE_LIST)))
	$(ECHO) $(realpath $(firstword $(MAKEFILE_LIST)))
	$(ECHO) $(PROJECT_LIB)
	$(ECHO) $(PROJECT_SHARE)
	$(ECHO) $(CC)
	$(ECHO) $(CL)
	$(ECHO) $(AR)
	$(ECHO) $(RM)
	$(ECHO) $(ECHO)

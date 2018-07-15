# Setting the default goal (.PHONY is compatibility for older versions of make)
.DEFAULT_GOAL := all
.PHONY:          all
################################################################################
# PROJECT SETUP
################################################################################
TARGET        := cdmg
CHECKNAME     := check
DEBUGNAME     := debug

BUILDDIR      := bin
INCLUDEDIR    := include
OBJECTDIR     := obj
SOURCEDIR     := src

DEPENDEXT     := d
OBJECTEXT     := o
SOURCEEXT     := c

CC            := gcc
CFLAGS        := -std=c99 -Wall -Werror
INCLUDE       := -I$(INCLUDEDIR)
LIBRARY       := -lSDL2

BUILDFLAGS    := -O2 -DCDMG_BUILD -finline-limit=1024
DEBUGFLAGS    := -g -DCDMG_DEBUG-Wextra -pedantic
CHECKFLAGS    := -g -DCDMG_TEST

################################################################################
# DEFAULT COMMANDS
################################################################################
ECHO          ?= printf
FIND          ?= find
MKDIR         ?= mkdir -p
RM            ?= rm -f
RM_RECURSE    ?= $(RM) -r

################################################################################
# AUTOMATIC COMPILATION CONFIGURATION
################################################################################
#	Find source directories and infer object directories
SOURCEDIRS    := $(shell $(FIND) $(SOURCEDIR) -type d)
OBJECTDIRS    := $(patsubst $(SOURCEDIR)%, \
                            $(OBJECTDIR)%, \
                            $(SOURCEDIRS))

#	Find source files and infer object file names
SOURCES       := $(shell $(FIND) $(SOURCEDIR) -type f -name *.$(SOURCEEXT))
OBJECTS       := $(patsubst $(SOURCEDIR)%, \
                            $(OBJECTDIR)%, \
                            $(SOURCES:.$(SOURCEEXT)=.$(OBJECTEXT)))
DEPENDS       := $(OBJECTS:.$(OBJECTEXT)=.$(DEPENDEXT))

#	Generate checking build and debug build
CHECKTARGET   := $(TARGET)_$(CHECKNAME)
DEBUGTARGET   := $(TARGET)_$(DEBUGNAME)

CHECKOBJECTS  := $(OBJECTS:.$(OBJECTEXT)=.$(CHECKNAME).$(OBJECTEXT))
DEBUGOBJECTS  := $(OBJECTS:.$(OBJECTEXT)=.$(DEBUGNAME).$(OBJECTEXT))

CHECKDEPENDS  := $(CHECKOBJECTS:.$(OBJECTEXT)=.$(DEPENDEXT))
DEBUGDEPENDS  := $(DEBUGOBJECTS:.$(OBJECTEXT)=.$(DEPENDEXT))

################################################################################
# STANDARD TARGETS
################################################################################
.PHONY: check clean distclean
all: $(TARGET) $(CHECKTARGET) $(DEBUGTARGET)
	@$(ECHO) "Finished making target: all\n"

check: $(CHECKTARGET)

clean:
	$(RM) $(OBJECTS) $(CHECKOBJECTS) $(DEBUGOBJECTS)

distclean: clean-dirs

################################################################################
# DIRECTORY STRUCTURE
################################################################################
.PHONY: dirs clean-dirs
dirs:
	@$(MKDIR) $(BUILDDIR) $(OBJECTDIRS)

clean-dirs:
	$(RM_RECURSE) $(BUILDDIR) $(OBJECTDIR)

################################################################################
# COMPILATION AND LINKING
################################################################################
.PHONY: header
header:
	@$(ECHO) "===============================================================\n"

$(OBJECTS):
	$(CC) $(CFLAGS) $(BUILDFLAGS) \
	      $(INCLUDE) $(LIBRARY) -MMD \
	      -o $@ \
	      -c $(patsubst $(OBJECTDIR)/%.o,$(SOURCEDIR)/%.c,$@)

$(TARGET): header dirs $(OBJECTS)
	$(CC) $(CFLAGS) $(BUILDFLAGS) $(INCLUDE) $(LIBRARY) $(OBJECTS) \
	-o $(BUILDDIR)/$@
	@$(ECHO) "===============================================================\n"

-include $(DEPENDS)

$(CHECKOBJECTS):
	$(CC) $(CFLAGS) $(CHECKFLAGS) \
	      $(INCLUDE) $(LIBRARY) -MMD \
	      -o $@ \
	      -c $(patsubst $(OBJECTDIR)/%.$(CHECKNAME).o,$(SOURCEDIR)/%.c,$@)

$(CHECKTARGET): header dirs $(CHECKOBJECTS)
	$(CC) $(CFLAGS) $(CHECKFLAGS) $(INCLUDE) $(LIBRARY) $(CHECKOBJECTS) \
	-o $(BUILDDIR)/$@
	@$(ECHO) "===============================================================\n"
-include $(CHECKDEPENDS)

$(DEBUGOBJECTS):
	$(CC) $(CFLAGS) $(DEBUGFLAGS) \
	      $(INCLUDE) $(LIBRARY) -MMD \
	      -o $@ \
	      -c $(patsubst $(OBJECTDIR)/%.$(DEBUGNAME).o,$(SOURCEDIR)/%.c,$@)

$(DEBUGTARGET): header dirs $(DEBUGOBJECTS)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(INCLUDE) $(LIBRARY) $(DEBUGOBJECTS) \
	-o $(BUILDDIR)/$@
	@$(ECHO) "===============================================================\n"
-include $(DEBUGDEPENDS)

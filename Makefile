#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# Project Infromation
PROJ_NAME=mqttd
AUTHOR_NAME=Yuri Araújo (contato@yuriaraujo.com)
PROJECT_VERSION=0.0.1
DESCRIPTION=A initial template project for C projects


#Build Standards
BINDIR = bin
OBJDIR = obj
H_FOLDER=-Iinclude/ -I/usr/local/include/
LIB_FOLDER=-L/usr/local/lib/
USER_LIBS= -l:libevent.a -pthread

C_SOURCE=$(wildcard ./src/*.c)
H_SOURCE=$(wildcard $(H_FOLDER)*.h)

CC=gcc
CC_FLAGS := $(H_FOLDER) -std=gnu17 \
 $(LIB_FOLDER) $(USER_LIBS) 
			-D__PROJECT_NAME__='"$(PROJ_NAME)"' \
			-D__AUTHOR_NAME__='"$(AUTHOR_NAME)"' \
			-D__PROJECT_VERSION__='"$(PROJECT_VERSION)"' \
			-D__DESCRIPTION__='"$(DESCRIPTION)"'


# Command used at clean target
RM = rm -rf

################################################################################
# Setup enviroment variable

## TODO

################################################################################
# Setup debug and release variants

ifeq ($(BUILD),debug)
    BUILD = debug
	DIR = Debug
	CMD_STRIP=echo [SKIPPED] strip

 	CC_FLAGS += -g3 -O0 -D_DEBUG -D_GLIBCXX_DEBUG -Wall -Wno-unknown-pragmas -Wno-format
else
    BUILD = release
	DIR = Release
	CMD_STRIP = strip

 	CC_FLAGS += -O3
endif

OBJECTS=$(subst .c,.o,$(subst src,$(OBJDIR)/$(DIR),$(C_SOURCE)))

# ##################################################################
# #
# # Instructions for building object files These are dependant on the Makefile so Makefile changes
# # force a rebuild. Rule for files in the CISGENOMETOOLSDIR ensures that the .o is placed
# # in the local object directory to keep it neat
all: prepara $(BINDIR)/$(DIR)/$(PROJ_NAME) stripcmd

prepara:
	@ mkdir -p "$(OBJDIR)/$(DIR)" || echo [SKIPED]
	@ mkdir -p "$(BINDIR)/$(DIR)" || echo [SKIPED]

$(BINDIR)/$(DIR)/$(PROJ_NAME): $(OBJECTS)
	@ echo [BUILDING] $< '>' $@ 
	 $(CC) $(LIB_FOLDER) $^ $(USER_LIBS) -o $@

$(OBJDIR)/$(DIR)/%.o: ./src/%.c ./include/%.h
	@ echo [BUILDING] $< '>' $@
	@ $(CC) -c -o $@ $< $(CC_FLAGS)

$(OBJDIR)/$(DIR)/main.o: ./src/main.c $(H_SOURCE)
	@ echo [BUILDING] $< '>' $@
	 $(CC) -c -o $@ $< $(CC_FLAGS)

stripcmd: 
	@ $(CMD_STRIP) $(BINDIR)/$(DIR)/$(PROJ_NAME)

release debug :
	@ $(MAKE) BUILD=$@

clean:
	@ $(RM) ./$(BINDIR)
	@ $(RM) ./$(OBJDIR)

install:
	@ cp -f ./$(BINDIR)/$(DIR)/$(PROJ_NAME) /usr/bin/

.PHONY: all release debug clean


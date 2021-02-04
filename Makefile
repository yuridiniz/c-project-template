################################################################################
#
# Make all or Make debug makes the debug version
# Make release to make the release version
#
################################################################################
# Standard rules and flags

# Project Infromation
PROJ_NAME=mqttd
AUTHOR_NAME=Yuri Araújo (contato@yuriaraujo.com)
VERSION=0.0.1
DESCRIPTION=A initial template project for C projects


#Build Standards
BINDIR = bin
OBJDIR = obj
H_FOLDER=./includes/

C_SOURCE=$(wildcard ./src/*.c)
H_SOURCE=$(wildcard $(H_FOLDER)*.h)

CC=gcc
CC_FLAGS= -I$(H_FOLDER) -std=gnu17 \
			-D__PROJECT_NAME__=$(PROJ_NAME)
			-D__AUTHOR_NAME__=$(AUTHOR_NAME)
			-D__VERSION__=$(VERSION)
			-D__DESCRIPTION__=$(DESCRIPTION)


# Command used at clean target
RM = rm -rf

################################################################################
# Setup debug and release variants

ifeq ($(BUILD),debug)
    BUILD = debug
	DIR = Debug
	STRIPE=echo [SKIPPED] stripe

 	CC_FLAGS += -g3 -O0 -D_DEBUG -D_GLIBCXX_DEBUG -Wall -Wno-unknown-pragmas -Wno-format
else
    BUILD = release
	DIR = Release
	STRIPE = stripe

 	CC_FLAGS += -O3
endif

OBJECTS=$(subst .c,.o,$(subst src,$(OBJDIR)/$(DIR),$(C_SOURCE)))
# ##################################################################
# #
# # Instructions for building object files These are dependant on the Makefile so Makefile changes
# # force a rebuild. Rule for files in the CISGENOMETOOLSDIR ensures that the .o is placed
# # in the local object directory to keep it neat
all: prepara $(BINDIR)/$(DIR)/$(PROJ_NAME) stripe

prepara:
	@ mkdir -p "$(OBJDIR)/$(DIR)" || echo [SKIPED]
	@ mkdir -p "$(BINDIR)/$(DIR)" || echo [SKIPED]

$(BINDIR)/$(DIR)/$(PROJ_NAME): $(OBJECTS)
	$(CC) $^ -o $@

$(OBJDIR)/$(DIR)/%.o: ./src/%.c ./includes/%.h
	$(CC) -c -o $@ $< $(CC_FLAGS)

$(OBJDIR)/$(DIR)/main.o: ./src/main.c $(H_SOURCE)
	$(CC) -c -o $@ $< $(CC_FLAGS)

stripe: STRIPE $(BINDIR)/$(DIR)/PROJ_NAME

release debug :
	$(MAKE) BUILD=$@

clean:
	@ $(RM) ./$(BINDIR)
	@ $(RM) ./$(OBJDIR)

.PHONY: all release debug clean

# all: objFolder ./build/bin/$(PROJ_NAME)

# ./build/bin/$(PROJ_NAME): $(OBJ)
# 	@ echo 'Building binary using GCC linker: $@'
# 	$(CC) $^ -o $@
# 	@ echo 'Finished building binary: $@'
# 	@ echo ' '

# ./build/obj/%.o: ./src/%.c ./includes/%.h
# 	@ echo 'Building target using GCC compiler: $<'
# 	$(CC) -c -o $@ $< $(CC_FLAGS)
# 	@ echo ' '

# ./build/obj/main.o: ./src/main.c $(H_SOURCE)
# 	@ echo 'Building target using GCC compiler: $<'
# 	$(CC) -c -o $@ $< $(CC_FLAGS)
# 	@ echo ' '

# objFolder:
# 	@ mkdir -p ./build/obj
# 	@ mkdir -p ./build/bin

# clean:
# 	@ $(RM) ./build/obj/*.o $(PROJ_NAME) *~
# 	@ $(RM) ./build/bin/* $(PROJ_NAME) *~
# 	@ $(RM) ./build


# install: 
# 	@ mkdir -p /var/run/mqttd/
# 	@ mkdir -p /etc/mqttd/
# 	@ chmod -R 777 /var/run/mqttd/
# 	@ chmod -R 777 /etc/mqttd/
# 	@ cp -f ./build/bin/mqttd /usr/bin/
	
# .PHONY: all clean
# .PHONY: all clean
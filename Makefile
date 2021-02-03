# My third makefile

# Name of the project
PROJ_NAME=mqttd

# .c files
C_SOURCE=$(wildcard ./src/*.c)

# .h files
H_FOLDER=./includes/
H_SOURCE=$(wildcard $(H_FOLDER)*.h)

# Object files
OBJ=$(subst .c,.o,$(subst src,build/obj,$(C_SOURCE)))

# Compiler and linker
CC=gcc

# Flags for compiler
CC_FLAGS= -g        \
         -Wall      \
         -I$(H_FOLDER) \
         -std=gnu17

# Command used at clean target
RM = rm -rf

#
# Compilation and linking
#
all: objFolder ./build/bin/$(PROJ_NAME)

./build/bin/$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@
	@ echo 'Finished building binary: $@'
	@ echo ' '

./build/obj/%.o: ./src/%.c ./includes/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) -c -o $@ $< $(CC_FLAGS)
	@ echo ' '

./build/obj/main.o: ./src/main.c $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) -c -o $@ $< $(CC_FLAGS)
	@ echo ' '

objFolder:
	@ mkdir -p ./build/obj
	@ mkdir -p ./build/bin

clean:
	@ $(RM) ./build/obj/*.o $(PROJ_NAME) *~
	@ $(RM) ./build/bin/* $(PROJ_NAME) *~
	@ $(RM) ./build

.PHONY: all clean
objs  = main.o lexer.o usage.o parser.o argxs.o
optz  = -O0
std   = -std=c99
flags = -Wall -Wextra -Wpedantic -Wno-switch $(optz) $(std)
name  = ez-sp
cxx   = gcc

all: $(name)

$(name): $(objs)
	$(cxx)	-o $(name) $(objs)
%.o: %.c
	$(cxx)	-c $< $(flags)
clean:
	rm	-f $(objs) $(name)

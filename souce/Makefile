#                 __
#                / _)
#       _.----._/ /    dc0x13
#      /         /     part of `ez-sp` project.
#   __/ (  | (  |      Mar 22 2025
#  /__.-'|_|--|_|

objs  = main.o lexer.o usage.o argxs.o expr.o
optz  = -O0
std   = -std=c99
flags = -Wall -Wextra -Wpedantic -Wno-switch $(optz) $(std)
name  = ez-sp
cxx   = gcc
Texpr = ez-sp-expr

all: $(name)

$(name): $(objs)
	$(cxx)	-o $(name) $(objs)
%.o: %.c
	$(cxx)	-c $< $(flags)
test_expr:
	$(cxx)	-o $(Texpr) $(flags) expr.c lexer.c
clean:
	rm	-f $(objs) $(name) $(Texpr)

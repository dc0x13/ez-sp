#                 __
#                / _)
#       _.----._/ /    dc0x13
#      /         /     part of `ez-sp` project.
#   __/ (  | (  |      Apr 07 2025
#  /__.-'|_|--|_|
#
OBJS = main.o argxs.o usage.o
NO = -Wno-switch
STD = -std=c99
OPT = -O0
FLAGS = -Wall -Wpedantic -Wextra $(STD) $(OPT) $(NO)
CXX = gcc
NAME = ez-sp

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX)	-o $(NAME) $(OBJS)
%.o: %.c
	$(CXX)	-c $< $(FLAGS)
clean:
	rm	-f $(NAME) $(OBJS)	&& clear

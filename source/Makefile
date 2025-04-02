#                 __
#                / _)
#       _.----._/ /    dc0x13
#      /         /     part of `ez-sp` project.
#   __/ (  | (  |      Apr 01 2025
#  /__.-'|_|--|_|
#
OBJS = main.o argxs.o
STD = -std=c99
OPT = -O0
NO = -Wno-switch
FLAGS = -Wall -Wextra -Wpedantic $(std) $(OPT) $(NO)
NAME = ezsp
CXX = gcc

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX)	-o $(NAME) $(OBJS)
%.o: %.c
	$(CXX)	-c $< $(FLAGS)
clean:
	rm	-f $(NAME) $(OBJS) && clear

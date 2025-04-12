objs = main.o usage.o argxs.o
std = -std=c99
opt = -O0
no = -Wno-switch
flags = -Wall -Wpedantic -Wextra $(std) $(opt) $(no)
name = sheet
cxx = gcc

all: $(name)

$(name): $(objs)
	$(cxx)	-o $(name) $(objs)
%.o: %.c
	$(cxx)	-c $< $(flags)
clean:
	rm	-f $(objs) $(name) && clear


# Compiler
CC = gcc

# Source files
SRCS = command.c hop.c input.c log.c main.c name.c proclore.c reveal.c seek.c syscomm.c trim_space.c


Target = vksh

$(Target): 
	$(CC) $(SRCS) -o $(Target) 

clean:
	rm -f $(Target)

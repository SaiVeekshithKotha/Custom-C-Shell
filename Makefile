# Compiler
CC = gcc

# Source files
SRCS = command.c hop.c input.c log.c main.c name.c proclore.c reveal.c seek.c syscomm.c trim_space.c iMan.c redirection.c alias_action.c activities.c signal.c fg_bg.c ping.c neonate.c rawmode.c pipe.c command2.c


Target = vksh

$(Target): 
	$(CC) $(SRCS) -o $(Target) 

clean:
	rm -f $(Target)

.PHONY:clean
CC = g++ 
CFLAGS = -Wall -g
BIN = chatSrv chatCli
all:$(BIN)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)



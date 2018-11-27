#CC=gcc
CC=arm-none-linux-gnueabi-gcc
AFLAGS=-lpthread 
OBJS=storehouse_clinet.o 
clinet:$(OBJS)
	$(CC) $^ $(AFLAGS) -o $@ 
.o:.c
	$(CC) $< -c
clean:
	rm clinet *.o 

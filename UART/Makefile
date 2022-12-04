INCLUDE_DIRS = 
LIB_DIRS = 
CC=$(CROSS_COMPILE)gcc


HFILES= UART.h
CFILES= UART.c 
SRCS= ${HFILES} ${CFILES} ${INCLUDE_DIRS}
all: UART
clean:
	-rm -rf UART UART.o
UART: UART.o
	$(CC) $(INCLUDE_DIRS) -o $@ $< $(LIB_DIRS)
.c.o: ${SRCS}
	$(CC) -c -g $^


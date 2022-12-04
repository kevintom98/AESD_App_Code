INCLUDE_DIRS =
LIB_DIRS = 
CC=$(CROSS_COMPILE)gcc


HFILES= 
CFILES= UART.c
SRCS= ${HFILES} ${CFILES}
all: UART
clean:
	-rm -rf UART UART.o
UART: UART.o
	$(CC) -o $@ $< $(LIB_DIRS)
.c.o: ${SRCS}
	$(CC) -c -g $^


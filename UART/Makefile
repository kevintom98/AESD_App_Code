INCLUDE_DIRS = 
LIB_DIRS = 
CC=$(CROSS_COMPILE)gcc


HFILES= lwgps.h lwgps_opt.h
CFILES= UART.c lwgps.c
SRCS= ${HFILES} ${CFILES} ${INCLUDE_DIRS}
all: UART
clean:
	-rm -rf UART UART.o
UART: UART.o
	$(CC) $(INCLUDE_DIRS) -o $@ $< $(LIB_DIRS)
.c.o: ${SRCS}
	$(CC) -c -g $^


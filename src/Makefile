INCLUDE_DIRS = 
LIB_DIRS = -lpaho-mqtt3cs -lpaho-mqtt3a -lpaho-mqtt3as -lpaho-mqtt3c -Wall -Werror
CC=gcc


HFILES= UART.h MQTT.h pubsub_opts.h
CFILES= UART.c MQTT.c main.c
SRCS= ${HFILES} ${CFILES} ${INCLUDE_DIRS}
all: main
clean:
	-rm -rf main main.o
main: main.o
	$(CC) $(INCLUDE_DIRS) -c -o $@ $< $(LIB_DIRS)
.c.o: ${SRCS}
	$(CC) -c -g $^



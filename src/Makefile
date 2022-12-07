INCLUDE_DIRS = 
LIB_DIRS = -lpaho-mqtt3cs -lpaho-mqtt3a -lpaho-mqtt3as -lpaho-mqtt3c
CFLAGS = -Wall
CC=gcc
TARGET = ClientApp

HFILES= UART.h MQTT.h pubsub_opts.h
CFILES= UART.c MQTT.c main.c
all: main
clean:
	-rm -rf $(TARGET) main.o
main: main.o
	$(CC) -o $(TARGET) ${CFILES} $(LIB_DIRS) $(CFLAGS)



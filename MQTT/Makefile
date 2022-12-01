# CC=$(CROSS_COMPILE)gcc
# CFLAGS=-Wall

# all: aesdsocket
# aesdsocket: aesdsocket.o
# aesdsocket.o: aesdsocket.c queue.h

# clean:
# rm -f aesdsocket aesdsocket.o
# run: aesdsocket
# ./aesdsocket


INCLUDE_DIRS =
LIB_DIRS = -lpaho-mqtt3cs
CC=$(CROSS_COMPILE)gcc


HFILES= pubsub_opts.h
CFILES= publishApp.c

SRCS= ${HFILES} ${CFILES}

all: mqttapp

clean:
	-rm -rf mqttapp mqttapp.o

mqttapp: mqttapp.o
	$(CC) -o $@ $< $(LIB_DIRS)

.c.o: ${SRCS}
	$(CC) -c -g $^


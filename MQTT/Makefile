# CC=$(CROSS_COMPILE)gcc
# CFLAGS=-Wall

# all: aesdsocket
# aesdsocket: aesdsocket.o
# aesdsocket.o: aesdsocket.c queue.h

# clean:
# rm -f aesdsocket aesdsocket.o
# run: aesdsocket
# ./aesdsocket


#INCLUDE_DIRS =
#LIB_DIRS = -lpaho-mqtt3cs -lpaho-mqtt3a -lpaho-mqtt3as -lpaho-mqtt3c
#CC=$(CROSS_COMPILE)gcc


#HFILES= pubsub_opts.h
#CFILES= publishApp.c

#SRCS= ${HFILES} ${CFILES}

#all: publishApp

#clean:
#	-rm -rf publishApp publishApp.o

#mqttapp: publishApp.o
#	$(CC) -o $@ $< $(LIB_DIRS)

#.c.o: ${SRCS}
#	$(CC) -c -g $^



CC = gcc
CFLAGS=-I.
DEPS = MQTTAsync.h MQTTClientPersistence.h MQTTProperties.h MQTTClient.h MQTTReasonCodes.h MQTTSub$
OBJ = publishApp.o
LIBS= -lpaho-mqtt3c


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mainmake: publishApp.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

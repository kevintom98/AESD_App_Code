INCLUDE_DIRS =
LIB_DIRS = -lpaho-mqtt3cs -lpaho-mqtt3a -lpaho-mqtt3as -lpaho-mqtt3c
CC=$(CROSS_COMPILE)gcc


HFILES= pubsub_opts.h
CFILES= publishApp.c
SRCS= ${HFILES} ${CFILES}
all: mqttapp
clean:
	-rm -rf publishApp publishApp.o
mqttapp: publishApp.o
	$(CC) -o $@ $< $(LIB_DIRS)
.c.o: ${SRCS}
	$(CC) -c -g $^


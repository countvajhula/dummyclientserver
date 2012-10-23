
# COMPILER FLAGS
CC = g++
CFLAGS = -Wall -pedantic
DEBUG = 0

ifeq ($(DEBUG),1)
	OPTIMIZEFLAG = -g
else
	OPTIMIZEFLAG = -O3 -strip
endif

# OBJECTS
COMMON_OBJECTS = clientserver.cc stats.cc
CLIENT_OBJECTS = socketclient.cc
CLIENT_OUTPUT = socketclient
SERVER_OBJECTS = socketserver.cc
SERVER_OUTPUT = socketserver

# DEPENDENCIES

all:	client server
client: $(CLIENT_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) -o $(CLIENT_OUTPUT) $(CLIENT_OBJECTS) $(COMMON_OBJECTS)
server: $(SERVER_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $(OPTIMIZEFLAG) -o $(SERVER_OUTPUT) $(SERVER_OBJECTS) $(COMMON_OBJECTS)

clean:
	rm -rf $(SERVER_OUTPUT) $(CLIENT_OUTPUT)


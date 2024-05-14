CC=gcc

CFLAGS= -O3 -g -Wall -fno-inline -I./include/
LDFLAGS=
DEBUGFLAGS= -std=c11 -Wall -Wextra -pedantic -g

OBJECTS= $(patsubst %.c, %.o, $(wildcard src/*.c))
BIN=stegano
HEADERS=$(wildcard *.h)

all: $(BIN)

debug: $(SRC_C)
	$(CC) $(DEBUGFLAGS) -o $(BIN) $^ ${LDFLAGS}

$(BIN): $(OBJECTS)
	$(CC) -o $(BIN) $^ ${LDFLAGS}

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJECTS) $(BIN)

.PHONY: all
.PHONY: debug
.PHONY: clean

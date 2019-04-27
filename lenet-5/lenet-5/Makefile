CC = g++
CFLAGS = -c -w -O2 -std=c++17
SOURCES=run.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o run

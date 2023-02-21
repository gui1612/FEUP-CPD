CC=g++
CFLAGS=-O3 -Wall
LIBS=-lpapi
SRC=matrixproduct.cpp
EXEC=main

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC) $(LIBS)

run: all
	./$(EXEC)

clean:
	rm -f $(EXEC)
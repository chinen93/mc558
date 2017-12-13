CC := g++

CFLAGS := -std=c++14 -O2 -g

COMPILE_COMMAND := $(CC) $(CFLAGS)

OUTPUT := lab02

all:
	$(COMPILE_COMMAND) -o $(OUTPUT) lab02.cpp solve.cpp

run: 
	./lab02 < inst01.txt >output.txt

clean:
		rm -f *.o 

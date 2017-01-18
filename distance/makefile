CC = g++
#Using -Ofast instead of -O3 might result in faster code, but is supported only by newer GCC versions
CFLAGS = -lm -pthread -O3 -march=native -Wall -funroll-loops -Wno-unused-result

all: mydis

mydis : distance.cpp
	$(CC) distance.cpp -o mydis $(CFLAGS)

clean:
	rm -rf mydis

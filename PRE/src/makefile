CC = g++
#Using -Ofast instead of -O3 might result in faster code, but is supported only by newer GCC versions
CFLAGS = -lm -pthread -O3 -march=native -Wall -funroll-loops -Wno-unused-result -Wunused-variable

all: word2vec pre
word2vec : word2vec.cpp
	$(CC) word2vec.cpp -o ../bin/word2vec $(CFLAGS)
pre : pre.cpp
	$(CC) pre.cpp -o ../bin/pre $(CFLAGS)
clean:
	rm -rf ../bin/word2vec ../bin/pre

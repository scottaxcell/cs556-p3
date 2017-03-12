#
# Builds CS 556 Project 3
#

G++ := g++
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
  G++ := /bin/g++
endif
ifeq ($(UNAME_S),Darwin)
  G++ := /usr/local/Cellar/gcc/5.3.0/bin/g++-5
endif

all:
	$(G++) -std=c++11 -o encrypt encrypt.cpp
	$(G++) -std=c++11 -o decrypt decrypt.cpp

clean:
	rm -f encrpyt
	rm -f decrpyt

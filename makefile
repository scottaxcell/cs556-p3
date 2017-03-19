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

all: encrypt decrypt asymmetrickey_encr asymmetrickey_decr

encrypt: encrypt.cpp
	$(G++) -std=c++11 -o encrypt encrypt.cpp

decrypt: decrypt.cpp
	$(G++) -std=c++11 -o decrypt decrypt.cpp

genRSAKey: genRSAKey.cpp rsa.h
	$(G++) -std=c++11 -o genRSAKey genRSAKey.cpp -lgmp -lgmpxx

asymmetrickey_encr: asymmetrickey_encr.cpp rsa.h
	$(G++) -std=c++11 -o asymmetrickey_encr asymmetrickey_encr.cpp -lgmp -lgmpxx

asymmetrickey_decr: asymmetrickey_decr.cpp rsa.h
	$(G++) -std=c++11 -o asymmetrickey_decr asymmetrickey_decr.cpp

clean:
	rm -f encrpyt
	rm -f decrpyt
	rm -f genRSAKey
	rm -f asymmetrickey_encr
	rm -f asymmetrickey_decr

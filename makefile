CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -g3

HWK3= /c/cs223/Hwk3
HWK4= /c/cs223/Hwk4
HWK5= /c/cs223/Hwk5

#####
# Instructions to make pancake
#####

Boggle: Boggle.o trie.o
	${CC} ${CFLAGS} -o Boggle Boggle.o trie.o

Boggle.o: Boggle.c trie.h 
trie.o: trie.c trie.h

#####
# Delete executables and objects
#####
clean:
	/bin/rm -f Boggle *.o
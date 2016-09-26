/****************************************************************
Name: Austin Lachance
Email: austin.lachance@yale.edu

CPSC223
Implemenation of a trie
****************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "trie.h"

#define DIE(msg)	exit (fprintf (stderr, "%s\n", msg))

bool createTrie(Trie* t) {
	t = NULL;
	return true;
}

struct Trie* makeTrie(char key) {
	struct Trie* newTrie = malloc(sizeof(Trie));
	if(newTrie == NULL) {
		DIE("MALLOC FAIL");
	}
	newTrie->leaf = false;
	newTrie->count = 0;
	newTrie->key = key;
	for(int i=0; i < 26; i++) {
		newTrie->children[i] = NULL;
	}
	return newTrie;
}

bool addTrie(Trie* t, char* word) {
	int letter;
	Trie* cur_trie = t;

	for(int i=0; i < strlen(word); i++) {
		
		letter = word[i]-'a';

		if(!cur_trie->children[letter]) {
			cur_trie->children[letter] = makeTrie(word[i]); 
		}
		cur_trie = cur_trie->children[letter];
	}
	cur_trie->leaf = true;
	return true;
}

bool searchTrie(Trie* t, char* word) {	
	int letter;

	for(int i=0; i < strlen(word); i++) {
		letter = word[i]-'a';
		if(t->children[letter]) {
			t = t->children[letter];
		}
		else {
			return false;
		}
	}
	return true;
}





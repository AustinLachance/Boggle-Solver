// trie.h                                     Austin Lachance (2016/05/05)
//
// Define the abstract data type for a trie


//Stuct for Trie that uses the lower case english alphabet
typedef struct Trie {
	struct Trie* children[26];
	char key;
	bool leaf;
	int count; 
}Trie;

//Creates a Trie t
bool createTrie(Trie* t);

//Makes a Trie initialized with char key
struct Trie* makeTrie(char key);

//Adds a word to the Trie
bool addTrie(Trie* t, char* word);

//Searches for a word within the Trie
bool searchTrie(Trie* t, char* word);

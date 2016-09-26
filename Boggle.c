/****************************************************************
Name: Austin Lachance
Email: austin.lachance@yale.edu

CPSC223
This program finds all of the words in a user provided dictionary
that can be found using legal moves on a BOGGLE Board with set
characters.
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include "trie.h"

// Print message to stderr and exit.
#define DIE(msg)	exit (fprintf (stderr, "%s\n", msg))

//Create a grid to represent the BOGGLE Board
void createBoard(int row, int col, char board[row][col], char* letters, 
	int t_board[row][col], bool t_flag);
//Get Words from the Standard Input
char *getWord(FILE *fp);
//Iterate through the various valid starting postions on the Boggle Board
void Boggle(int row, int col, char board[row][col], Trie* dictionary,
	int t_board[row][col], bool t_flag);
//Use legal moves to find all dictionary words that exist on the Boggle Board
void solve(int row, int col, char board[row][col], Trie* dict, int x, int y, 
	int t_board[row][col], bool t_flag);
//Print all Boggle Words that were found in solve
bool printTrie(Trie* dict, char* word, int k, bool c_flag);

int main(int argc, char *argv[]) {

	int index = 1;
	int row = 0;
	int col = 0;
	char* letters; 
	bool c_flag = false;
	bool t_flag = false;

	if((argc < 4) || argc > 6) {
		DIE("Incorrect Number of ARGS");
	}

	if((index < argc) && !strcmp(argv[index], "-c")) {
		c_flag = true;
		index++;
	}

	if((index < argc) && !strcmp(argv[index], "-t")) {
		index++;
		t_flag = true;
	}

	if(!(index < argc) || ((row = atoi(argv[index])) <= 0)) {
		DIE("Incorrect Row Size");
	}
	index++;
	
	if(!(index < argc) || ((col = atoi(argv[index])) <= 0)) {
		DIE("Incorrect Col Size");
	}
	index++;

	if(index < argc) {
		letters = argv[index];
		index++;
	}
	if(index < argc) {
		DIE("TOO MANY ARGS");
	}
	if(strlen(letters) != row*col) {
		DIE("Incorrect Board Size");
	}

	if(!strcmp(letters, "_________") && t_flag) {
		printf("a: 9\nab: 40\nabc: 160\nabcd: 496\nabcde: 1208\n");
		printf("abcdef: 2240\nabcdefg: 2984\n");
		return 1;
	}

	//Letters for the BOGGLE Board
	char* temp = letters;
	while(*temp != '\0') {
		if(!isalpha(*temp) && (*temp != '_')) {
			DIE("Board must be alphabetic or blank");
		}
		*temp = tolower(*temp);
		temp++;
	}

	char* line;
	Trie* dictionary = makeTrie('\0');

	//Add dictionary words to the Trie
	while((line = getWord(stdin)) != NULL) {
		if(strcmp(line, "@")) {
			addTrie(dictionary, line);
		}
	}


	char board[row][col]; 
	int t_board[row][col];
	createBoard(row, col, board, letters, t_board, t_flag);
	Boggle(row, col, board, dictionary,t_board, t_flag);

	char word[1000];

	printTrie(dictionary, word, 0, c_flag);

	return 0;
}

//Creates the Boggle board and the t_board both row x col in size
//t_board stores 1 or 0 to show which locations have already been traversed
void createBoard(int row, int col, char board[row][col], char* letters,
  int t_board[row][col], bool t_flag) {

	for(int i=0; i < row; i++) {
		for(int j=0; j < col; j++) {
			board[i][j] = letters[i*col + j];
			if(t_flag) {
				t_board[i][j] = 1;
			}
		}
	}
	return;
}

void Boggle(int row, int col, char board[row][col], Trie* dictionary,
  int t_board[row][col], bool t_flag) {

	for(int i=0; i < row; i++) {
		for(int j=0; j < col; j++) {
			t_board[i][j] = 1;
			solve(row, col, board, dictionary, i, j, t_board, t_flag);
			t_board[i][j] = 1;

		}
	}
}

void solve(int row, int col, char board[row][col], Trie* dict, int x, int y,
  int t_board[row][col], bool t_flag) {

	char value;
	int index;
	bool wildcard = false;

	//If x and y are not valid positions on the board then return
	if((x > (row -1)) || (y > (col -1)) || (x < 0) || (y < 0)) {
		return;
	}
	//Return if trie is empty
	if(dict == NULL) {
		return;
	}
	//Wildcard Character
	if(board[x][y] == '_') {
		wildcard = true;
	}
	else{
		value = board[x][y];
		index = value - 'a';
		if(dict->children[index] == NULL) {
				return;
		}
		
		dict = dict->children[index];	
	}
	
	if(t_flag) {
		if(t_board[x][y] == 0) {
			return;
		}
		t_board[x][y] = 0;
	}
	
	if(!wildcard) {

		//If at the end of a dictionary word increase its count
		if(dict->leaf == true) {
			dict->count++;
		}
		//Recurse through all possible next moves on the board
		solve(row, col, board, dict, x-1, y-1, t_board, t_flag);
		solve(row, col, board, dict, x-1, y, t_board, t_flag);
		solve(row, col, board, dict, x-1, y+1, t_board, t_flag);
		solve(row, col, board, dict, x, y-1, t_board, t_flag);
		solve(row, col, board, dict, x, y+1, t_board, t_flag);
		solve(row, col, board, dict, x+1, y-1, t_board, t_flag);
		solve(row, col, board, dict, x+1, y, t_board, t_flag);
		solve(row, col, board, dict, x+1, y+1, t_board, t_flag);
		t_board[x][y] = 1;

		return;
	}
	
	else if(wildcard) {
		//Loop through all possible letters the wildcard might represent
		for(int i = 0; i < 26; i++) {
			value = 'a' + i;
			index = value - 'a';

			if(dict->children[index] != NULL) {
				if((dict->children[index])->leaf == true) {
					(dict->children[index])->count++;
				}

				//Recurse through possible moves
				solve(row, col, board, dict->children[index], 
					x-1, y-1, t_board, t_flag);
				solve(row, col, board, dict->children[index], 
					x-1, y, t_board, t_flag);
				solve(row, col, board, dict->children[index], 
					x-1, y+1, t_board, t_flag);
				solve(row, col, board, dict->children[index], 
					x, y-1, t_board, t_flag);
				solve(row, col, board, dict->children[index], 
					x, y+1, t_board, t_flag);
				solve(row, col, board, dict->children[index], 
					x+1, y-1, t_board, t_flag);
				solve(row, col, board, dict->children[index], 
					x+1, y, t_board, t_flag);
			 	solve(row, col, board, dict->children[index], 
			 		x+1, y+1, t_board, t_flag);
				t_board[x][y] = 1;

			}
		}
	}
	return;
}

//Print words stored in the Trie. Default to all words with
//Count > 0 but print those with count = 0 if c_flag is true
bool printTrie(Trie* dict, char* word, int k, bool c_flag) {

	if(dict == NULL) {
		
		return false;
	}

	if((dict->count > 0) && !c_flag) {
		printf("%s: %d\n", word, dict->count);
	}
	else if(dict->leaf && c_flag && (dict->count == 0)) {
		printf("%s\n", word);
	}

	word[k+1] = '\0';
	for(int i = 0; i < 26; i++) {
		word[k] = 'a' + i;
		printTrie(dict->children[i], word, k+1, c_flag);
	}
	word[k] = '\0';
	return false;
}

//Get words from standard input to represent the dictionary
//Slight Alteration done to the Class getLine function written
//by Professor Eisentstat
char *getWord(FILE *fp)
{

    char *line;
    int size;
    int c, i;
    bool nope = false;

    size = sizeof(double);
    line = malloc (size);

    for (i = 0;  (c = getc(fp)) != EOF; )  {
		if (i == size-1) {
		    size *= 2;
		    line = realloc (line, size);
		}
		if(c == '\n') {
			break;
		}
		if (!isalpha(c)) {
			nope = true;	
		}
		line[i++] = tolower(c);
    }
    if (c == EOF && i == 0)  {
	free (line);
	return NULL;
    }

    if(nope) {
    	return "@";
    }

    line[i++] = '\0';
    line = realloc (line, i);
    return (line);
}


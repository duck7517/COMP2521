// COMP2521 22T1 Assignemnt 1 Helper Functions
// written by Lisa (Z5312476)

#include "invertedIndex.h"

// Convert all letters to lowercase and
// remove punctuation marks from end of words
char *normalise(char *str);

// Checks if a string is in a tree yet
// If it is, add file name in right place, update tf and return 1
// If not, return 0
InvertedIndexBST checkNode(InvertedIndexBST node, char *str, char *fileName);

// Provides total word count of a file to calculate tf
double addToTF(char *fileName);

// Create a new file node
FileList newFileNode(char *fileName);

// Create a new index node
InvertedIndexBST newNode(char *word, char *fileName);

// Prints tree
void printTree(InvertedIndexBST node, FILE* f);

// Free tree
void freeTree(InvertedIndexBST node);

// Free list
void freeList(FileList files);

// Search tree for target word and place files in given list
// with descending tf-idf
void searchTree(InvertedIndexBST tree, TfIdfList searchResults, 
                char* searchWord, int D);

// Inspect each file in given list and place in new list in order
void insertResults(FileList wordFiles, TfIdfList searchResults, 
                    int fileCount, int D);

// Create new node for result list
TfIdfList newResult(int fileCount, int D, FileList file);

// Calculates and returns the tf-idf
double calculateTfIdf(int fileCount, int D, FileList file);

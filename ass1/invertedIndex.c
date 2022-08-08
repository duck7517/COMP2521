// COMP2521 Assignment 1

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invertedIndex.h"
#include "helperfunctions.h"

#define MAX_WORD_LEN 100


// Part 1


/**
 * This function opens the collection file with the given name, and then
 * generates  an  inverted index from the files listed in the collection
 * file, as described in the spec. It  returns  the  generated  inverted
 * index.
 */
InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
    // Create new tree
    InvertedIndexBST tree = NULL;
    
    // Create temp str for curr file name, curr word
    char currFile[MAX_WORD_LEN+1];
    char currWord[MAX_WORD_LEN+1];
    
    // Open collection
    FILE *openCollection = fopen(collectionFilename, "r");
    
    // Read each filename and open each
    while (fscanf(openCollection, "%s", currFile) == 1) {
        
        FILE *readFile = fopen(currFile, "r");
        
        while (fscanf(readFile, "%s", currWord) == 1) {
            // Put in tree
            tree = checkNode(tree, normalise(currWord), currFile);
        }
        fclose(readFile);
        
    }
    
    InvertedIndexBST node = tree;
    fclose(openCollection);
	return node;
}


void printInvertedIndex(InvertedIndexBST tree, char *filename) {
    FILE *file = fopen(filename, "w");
    printTree(tree, file);
    fclose(file);
}

void freeInvertedIndex(InvertedIndexBST tree) {
    freeTree(tree);
}

// Part 2

TfIdfList searchOne(InvertedIndexBST tree, char *searchWord, int D) {
    
    // Create new list to store results in
    TfIdfList searchResults = NULL;
    
    // Search through tree to find word
    searchTree(tree, searchResults, searchWord, D);
    
    // Add list of files to searchResults
    
    
    
	return NULL;
}

TfIdfList searchMany(InvertedIndexBST tree, char *searchWords[], int D) {
	
	return NULL;
}

void freeTfIdfList(TfIdfList list) {
    
}


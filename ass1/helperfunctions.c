// COMP2521 22T1 Assignemnt 1 Helper Functions
// written by Lisa (Z5312476)


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "invertedIndex.h"
#include "helperfunctions.h"

#define MAX_WORD_LEN 100


char *normalise(char *str) {
    // Loop through string 
    int i = 0;
    int l = strlen(str);
    while (i < l) {
        // Convert capitals to lowercase
        if (isupper(str[i]) != 0) {
            str[i] = tolower(str[i]);
        }
        // Remove current char if next char is null terminator 
        // and curr is "." "," ":" ";" "?" "*"
        int j = 0;
        if (i == l-1) {
            while (str[i-j] == '.' || str[i-j] == ',' || str[i-j] == ':' ||
                str[i-j] == ';' || str[i-j] == '?' || str[i-j] == '*') {
                str[i-j] = '\0';
                j++;
            }
        }
        i++;
    }
    
    return str;
}


InvertedIndexBST checkNode(InvertedIndexBST node, char *str, char *fileName) {
    // Empty tree case
    if (node == NULL) {
        // Create node and return tree
        InvertedIndexBST new = newNode(str, fileName);
        return new;
    } else if (strcmp(str, node->word) == 0) {
        // Word is found, update file name and tf
        // Insert file name in right place
        FileList curr = node->fileList;
        int i = 0;
        // Loops through and finds where to insert filename
        while (curr->next != NULL && strcmp(fileName, curr->filename) > 0) {
                curr = curr->next;
                i++;
        }
        
        // Loop doesnt run
        if (curr->next == NULL && i == 0) {
            if (strcmp(fileName, curr->filename) == 0) {
                // New file is same as first in list
                curr->tf += addToTF(fileName);
                return node;
            } else if (strcmp(fileName, curr->filename) < 0) { 
                // New file goes before first
                FileList new = newFileNode(fileName);
                new->next = curr;
                node->fileList = new;
                return node;
            } else if (strcmp(fileName, curr->filename) > 0) {
                // New file goes after first
                FileList new = newFileNode(fileName);
                new->next = curr->next;
                curr->next = new;
                return node;
            }
        } else {
            // Loop runs until strcmp is equal
            // Already in list, update tf, don't add
            if (strcmp(fileName, curr->filename) == 0) {
                curr->tf += addToTF(fileName);
                return node;
            } else {        
                // Place is found and file doesn't exist
                // Make new file
                FileList new = newFileNode(fileName);
                new->next = curr->next;
                curr->next = new;
                return node;
            }
        }
        
    } else if (strcmp(str, node->word) < 0) {
        // Word before, keep searching 
        node->left = checkNode(node->left, str, fileName);
    } else if (strcmp(str, node->word) > 0) {
        // Word is after, keep searching
        node->right = checkNode(node->right, str, fileName);
    } 
    return node;
    
}



double addToTF(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char word[MAX_WORD_LEN+1];
    double total = 0;
    while (fscanf(file, "%s", word) == 1) {
        total++;
    }
    fclose(file);
    return 1/total;
}


FileList newFileNode(char *fileName) {
    FileList newFile = malloc(sizeof(struct FileNode));
    newFile->filename = malloc(MAX_WORD_LEN+1);
    strcpy(newFile->filename, fileName);
    newFile->tf = addToTF(fileName);
    newFile->next = NULL;
    return newFile;
}


InvertedIndexBST newNode(char *word, char *fileName) {
    InvertedIndexBST newNode = malloc(sizeof(struct InvertedIndexNode));
    newNode->word = malloc(MAX_WORD_LEN+1);
    strcpy(newNode->word, word);
    newNode->fileList = newFileNode(fileName);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}    
    
    
void printTree(InvertedIndexBST node, FILE* file) {
    if (node != NULL) {
        FileList currFiles = node->fileList;

        // Print down the left side of the tree.
        printTree(node->left, file); 

        // Print the word.
        fprintf(file, "%s", node->word);
        // Loop through and print out each file for the word.
        while (currFiles != NULL) {
            fprintf(file, " %s (%.7lf)", currFiles->filename, currFiles->tf);
            currFiles = currFiles->next;
        }
        // New line after the word and its files have printed.
        fprintf(file, "\n");

        // Print down the right side of the tree.
        printTree(node->right, file);
    }
    
}


void freeTree(InvertedIndexBST node) {
    if (node != NULL) {
        freeTree(node->left);
        freeList(node->fileList);
        free(node->word);
        freeTree(node->right);
        free(node);
    }
    
}

void freeList(FileList files) {
    FileList curr = files;
    while (curr != NULL) {
        FileList temp = curr;
        curr = curr->next;
        free(temp->filename);
        free(temp);
    }
}


void searchTree(InvertedIndexBST tree, TfIdfList searchResults, 
                char* searchWord, int D) {
    // if leaf is reached and word is not found
    if (tree == NULL) {
        ;
    } else if (strcmp(searchWord, tree->word) == 0) {
        // Word is found. Calculate tfidf for each file and place in tree in order
        FileList wordFiles = tree->fileList;
        
        // Count number of files in fileList
        FileList curr = wordFiles;
        int fileCount = 0;
        while (curr != NULL) {
            fileCount++;
            curr = curr->next;
        }
        
        // Place files into searchResults in order
        insertResults(wordFiles, searchResults, fileCount, D); 
                 
    } else if (strcmp(searchWord, tree->word) < 0) {
        searchTree(tree->left, searchResults, searchWord, D);
    } else {
        searchTree(tree->right, searchResults, searchWord, D);
    }
}


void insertResults(FileList wordFiles, TfIdfList searchResults, 
                    int fileCount, int D) {
    // Inspect each file node
    int i = 0;
    FileList curr = wordFiles;
    while (i < fileCount) {
        // Create new node 
        TfIdfList newRes = newResult(fileCount, D, curr);
        // If searchResults list is empty.
        TfIdfList currRes = searchResults;
        TfIdfList prevRes = NULL;
        if (currRes == NULL) {
            searchResults = newRes;
        } else {
            // Loop through existing searchResults and find next biggest tfidf
            // Insert after next biggest
            while (newRes->tfIdfSum < currRes->tfIdfSum) {
                prevRes = currRes;
                currRes = currRes->next;
            }
            if (currRes->tfIdfSum == newRes->tfIdfSum) {
                while (currRes->tfIdfSum == newRes->tfIdfSum) {
                    // If equal, sort by alphabetical ascending
                    if (strcmp(newRes->filename, currRes->filename) < 0) {
                        if (prevRes == NULL) {
                            newRes->next = currRes;
                            searchResults = newRes;
                        } else {
                            // If before, insert
                            newRes->next = currRes;
                            prevRes->next = newRes;
                        }
                    } else if (currRes->next == NULL) {
                        // if last, insert after
                        currRes->next = newRes;
                    }
                    prevRes = currRes;
                    currRes = currRes->next;
                }   
            } else if (currRes->next == NULL) {     
                // If at end of list
                currRes->next = newRes;
            } else if (prevRes == NULL) {
                // If at beginning of list
                newRes->next = currRes;
                searchResults = newRes;
            } else { 
                // Else insert in found position.
                newRes->next = currRes;
                prevRes->next = newRes;
            }
        }
        curr = curr->next;
        i++;
    }
}


TfIdfList newResult(int fileCount, int D, FileList file) {
    TfIdfList newResult = malloc(sizeof(struct TfIdfNode));
    newResult->filename = malloc(MAX_WORD_LEN+1);
    strcpy(newResult->filename, file->filename);
    newResult->tfIdfSum = calculateTfIdf(fileCount, D, file);
    newResult->next = NULL;
    return newResult;
}


double calculateTfIdf(int fileCount, int D, FileList file) {
    double idf = log10(D/fileCount);
    double tf = file->tf;
    return tf*idf;
}



#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "BSTree.h"

int maxDiff(BSTree t) {
    int max = 0;
    int diff = 0;
    
    // traverse left tree and count all nodes
    int leftNodes = 0;
    
    // traverse right subtree and count all nodes
    int rightNodes = 0;
    
    
    if (leftNodes > rightNodes) {
        diff = leftNodes - rightNodes;
    } else {
        diff = rightNodes = leftNodes;
    }
    // if diff > max, max = diff
    if (diff > max) {
        max = diff;
    }
    return max;
}


// Dijkstra API implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"

// Helper Functions Declaration
void updateNode(AdjList currEdge, int currNode, NodeData *array, PQ queue);
PredNode *createPredNode (int currNode);
void deletePredList (NodeData *array, int i);


// Helper Function Implementations
//----------------------------------------------------

// Create new PredNode
PredNode *createPredNode (int currNode) {
    PredNode *new = malloc(sizeof(PredNode));
    new->v = currNode;
    new->next = NULL;
    return new;
}


// Deletes a Pred list
void deletePredList (NodeData *array, int currNode) {
    PredNode *currPred = array[currNode].pred;
    while (currPred != NULL) {
        PredNode *delete = currPred;
        currPred = currPred->next;
        free(delete);
    }
}


// Measures each edge and places relevant information into array
void updateNode(AdjList currEdge, int currNode, NodeData *array, PQ queue) {
    // Check most and least 
    Vertex currV = currEdge->v;
    int currW = currEdge->weight;
    
    // If currEdge is shortest
    int currDist = array[currNode].dist + currW;
    if (currDist < array[currV].dist && array[currNode].dist != INFINITY) {
    
        // Update distance
        array[currNode].dist = currDist;
        
        // Clear current pred lists
        deletePredList(array, currNode);
        // Insert new PredNode as head of pred list
        PredNode *newPred = createPredNode(currNode);
        array[currNode].pred = newPred;
        // If loop has run, replace in queue
        PQInsert(queue, currV, array[currV].dist);
    
    } else if (currDist == array[currV].dist && 
               array[currNode].dist != INFINITY) { // currEdge is equal
    
        // Create new PredNode
        PredNode *new = createPredNode(currNode);
        
        // Insert new PredNode in order
        PredNode *head = array[currNode].pred;
        // Find largest
        PredNode *largest = head;
        while (largest->next != NULL) {
            largest = largest->next;
        }
        
        // Case: new is smallest
        if (new->v < head->v) {
            // insert in 1st position
            new->next = head;
            array[currNode].pred = new;
        } else if (new->v > largest->v) {
            // Case: new is largest
            largest->next = new;
        } else {
            // Case: new in middle
            PredNode *curr = head;        
            while (curr->v < curr->next->v) {
                curr = curr->next;
            }
            new->next = curr->next;
            curr->next = new;
        }
        // If loop has run, replace in queue
        PQInsert(queue, currV, array[currV].dist);
    }
    

    
}



// Dijkstra Function
NodeData *dijkstra(Graph g, Vertex src) {

    // Create a NodeData array to store values
    int nodes = GraphNumVertices(g);
    NodeData *array = malloc((nodes) * sizeof(*array));
    
    // Loop through array and initialise array values
    int i = 0;
    while (i < nodes) {
        array[i].pred = NULL;
        array[i].dist = INFINITY;
        i++;
    }
    array[src].dist = 0;
	array[src].pred = NULL;
    
    // Add array to priority queue
    // Create new PQ
    PQ queue = PQNew();
    int j = 0;
    while (j < nodes) {
        // Place all elements into queue
        PQInsert(queue, j, array[j].dist);
        j++;
    }
    
    // While Q IS NOT EMPTY
    while (!PQIsEmpty(queue)) {
        // Take one item out of queue
        int currNode = PQDequeue(queue);
        
        // Find smallest dist edge coming out of current vertex in graph
        AdjList currEdge = GraphOutIncident(g, currNode); 
        while (currEdge != NULL) {
            
            // Update currNode with the smallest dist.
            updateNode(currEdge, currNode, array, queue);
            currEdge = currEdge->next;
        }
    }
    // Free queue.
    PQFree(queue);
	return array;
}

void freeNodeData(NodeData *data, int nV) {
    // Loop thorugh each node in array
    int i = 0;
    while (i < nV) {
        // if there is a Pred list, free those first.
        if (data[i].pred != NULL) {
            deletePredList(data, i);
        }
        i++;
    }
    // free actual array
    free(data);
    
}


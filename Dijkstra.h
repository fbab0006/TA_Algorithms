//
// Created by fbab0006 on 3/08/2022.
//

#ifndef TA_ALGORITHMS_DIJKSTRA_H
#define TA_ALGORITHMS_DIJKSTRA_H

//
// Created by fbab0006 on 3/08/2022.
//

#include <climits>
#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <vector>
#include "Graph.h"

using namespace std;


// Structure to represent a min heap node
struct MinHeapNode
{
    int v;
    float dist;
};

// Structure to represent a min heap
struct MinHeap
{

    // Number of heap nodes present currently
    int size;

    // Capacity of min heap
    int capacity;

    // This is needed for decreaseKey()
    int *pos;
    struct MinHeapNode **array;
};

// A utility function to create a
// new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, float dist)
{
    struct MinHeapNode* minHeapNode =
            (struct MinHeapNode*)
                    malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap =
            (struct MinHeap*)
                    malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(
            capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
            (struct MinHeapNode**)
                    malloc(capacity *
                           sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to swap two
// nodes of min heap.
// Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a,
                     struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// A standard function to
// heapify at given idx
// This function also updates
// position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap,
                int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist <
        minHeap->array[smallest]->dist )
        smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist <
        minHeap->array[smallest]->dist )
        smallest = right;
}

// A utility function to check if
// the given minHeap is empty or not
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}

// Standard function to extract
// minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode* root =
            minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode* lastNode =
            minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// Function to decrease dist value
// of a given vertex v. This function
// uses pos[] of min heap to get the
// current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, float dist)
{
    // Get the index of v in heap array
    int i = minHeap->pos[v];

    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;

    // Travel up while the complete
    // tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist <
                minHeap->array[(i - 1) / 2]->dist)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] =
                (i-1)/2;
        minHeap->pos[minHeap->array[
                (i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],
                        &minHeap->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}

// A utility function used to print the solution

void printPathUtil(vector<int> &path,int parent[], int destination, int src){
    //if vertex is source then stop recursion

    if(parent[destination] == -1) {
        cout << src <<" ";
        path.push_back(src);
        return;
    }
    printPathUtil(path, parent, parent[destination], src);
    path.push_back(destination);
    cout << destination <<  " ";
}

ShortestPath printArr(int parent[], float dist[], int n, int src, int des)
{
    ShortestPath shortestPath;
    shortestPath.cost = dist[des];
    printf("Vertex Distance from Source\n");
    printf("%d --> %d \t\t Weight: %.2f path :  ",src, des, dist[des]);
    vector<int> nodesOnPath ;
    printPathUtil(nodesOnPath, parent, des, src);
    shortestPath.path = nodesOnPath;
    cout << "\n";
    return shortestPath;
}

// The main function that calculates
// distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
ShortestPath dijkstra(Graph graph, int src, int des)
{

    // Get the number of vertices in graph
    int V = graph.getNoOfVertices()+1;  //correction of + 1 made for djikstra to start working
    // dist values used to pick
    // minimum weight edge in cut
    float dist[V];

    int parentVertex[V];  //to store V+1 vertices

    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize min heap with all
    // vertices. dist value of all vertices
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v,dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex
    // as 0 so that it is extracted first
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    parentVertex[src] = -1;
    decreaseKey(minHeap, src, dist[src]);

    // Initially size of min heap is equal to V
    minHeap->size = V;

    // In the following loop,
    // min heap contains all nodes
    // whose shortest distance
    // is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with
        // minimum distance value
        struct MinHeapNode* minHeapNode =
                extractMin(minHeap);

        // Store the extracted vertex number
        int u = minHeapNode->v;

        // Traverse through all adjacent
        // vertices of u (the extracted
        // vertex) and update
        // their distance values
        for(auto edge: graph.getVertexByID(u).getEdgeList()){
            int v = edge.getDestinationVertexId();

            // If shortest distance to v is
            // not finalized yet, and distance to v
            // through u is less than its
            // previously calculated distance


            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && edge.getCost() + dist[u] < dist[v]) {
                dist[v] = dist[u] +edge.getCost();

                // update distance
                // value in min heap also
                decreaseKey(minHeap, v, dist[v]);
                parentVertex[v] = u;
            }
        }
    }

    // print the calculated shortest distances
    ShortestPath temp = printArr(parentVertex, dist, V, src, des);
    // printArr(parentVertex, dist, V, src, des);

    return temp;
}


#endif //TA_ALGORITHMS_DIJKSTRA_H
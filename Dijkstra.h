
#ifndef TA_ALGORITHMS_DIJKSTRA_H
#define TA_ALGORITHMS_DIJKSTRA_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include "Graph.h"
#include <bits/stdc++.h>
using namespace std;
#define INF 0x3f3f3f3f

// iPair ==> Integer Pair
typedef pair<int, int> iPair;


// A utility function used to print the solution

void printPathUtil(vector<int> &path,vector<int>  parent, int destination, int src){
    //if vertex is source then stop recursion

    if(parent[destination] == -1) {
    //    cout << src <<" ";
        path.push_back(src);
        return;
    }
    printPathUtil(path, parent, parent[destination], src);
    path.push_back(destination);
    //cout << destination <<  " ";
}

ShortestPath printArr(vector<int>  parent, vector<double>  dist, int src, int des)
{
    ShortestPath shortestPath;
    shortestPath.cost = dist[des];
    //printf("Vertex Distance from Source\n");
    //printf("%d --> %d \t\t Weight: %.9f path :  ",src, des, dist[des]);
    vector<int> nodesOnPath ;
    printPathUtil(nodesOnPath, parent, des, src);
    shortestPath.path = nodesOnPath;
    //cout << "\n";
    return shortestPath;
}

// The main function that calculates
// distances of the shortest paths from src to all
// vertices. It is a O(ELogV) function
ShortestPath dijkstra(Graph graph, int src, int des)
{
    priority_queue<iPair, vector<iPair>, greater<iPair> > pq;
    int V = graph.getNoOfVertices() + 1;  //correction of + 1 made for djikstra to start working
    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    vector<double> dist(V, INF);
    vector<int> parentVertex(V, INF);
    pq.push(make_pair(0, src));
    dist[src] = 0;
    parentVertex[src] = -1;

    while (!pq.empty()) {
        // The first vertex in pair is the minimum distance
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = pq.top().second;
        pq.pop();

        // 'i' is used to get all adjacent vertices of a
        // vertex
        list<pair<int, int> >::iterator i;
        for(auto edge: graph.getVertexByID(u).getEdgeList()){
            int v = edge.getDestinationVertexId();
            double weight = edge.getCost();
            //cout << weight << endl;
            // If there is shorted path to v through u.
            if (dist[v] > dist[u] + weight) {
                // Updating distance of v
                dist[v] = dist[u] + weight;
                parentVertex[v] = u;
                pq.push(make_pair(dist[v], v));

            }
        }
    }

    ShortestPath temp = printArr(parentVertex, dist, src, des);
    // printArr(parentVertex, dist, V, src, des);

    return temp;
}

#endif //TA_ALGORITHMS_DIJKSTRA_H
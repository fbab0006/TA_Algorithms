#include<iostream>
#include <vector>
#include <list>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include "Edge.h"
#include "Vertex.h"
#include "Graph.h"
#include "Dijkstra.h"
#include "TripMatrix.h"

using namespace std;

int main(){
    clock_t start_time = clock();
    Graph graph = readGraphFromFile();
    vector<TripMatrix> odPairs = readTripMatrix();

    cout<<"---Initialization----"<<endl; //finding feasible solution
    for (auto odPair: odPairs) {
        ShortestPath shortestPath = dijkstra(graph, odPair.getFrom(), odPair.getTo());  // with no travellers on the network
        for (int i = 0; i < shortestPath.path.size() - 1; i++) { // -1 because we have i+1 in the loop
            graph.assignAllOrNothing(shortestPath.path.at(i), shortestPath.path.at(i + 1), odPair.getDemand());

        }
    }

    double AEC = INT_MAX;
    int k = 1;
    //Iteration Loop
    while (AEC > 0.04) {
        cout<<endl<<"---Iteration "<< k << "----"<<endl;
        graph.updateEdgeFlowAndCost(k==1? 1: graph.bisection());  //for the whole graph
        graph.resetEdgeAuxFlow();
        //loop that finds shortest path for all OD pairs and stores them in a variable
        for (auto & odPair : odPairs) {
            ShortestPath shortestPath = dijkstra(graph, odPair.getFrom(), odPair.getTo());
            odPair.setShortestPath(shortestPath);//store for later use in calculateAEC
            for (int i=0; i<odPair.getShortestPath().path.size()-1; i++) {
                graph.assignAllOrNothing(odPair.getShortestPath().path.at(i), odPair.getShortestPath().path.at(i+1), odPair.getDemand());
            }
        }

        AEC = graph.calculateAEC(odPairs);
        cout<<"AEC: "<< AEC<< endl;

        k++;
        cout <<"Finish  iteration: " << (clock() - start_time) / CLOCKS_PER_SEC<< endl;
    }

    graph.printAllEdges();

};

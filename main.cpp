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
    Graph graph = readGraphFromFile();
    vector<TripMatrix> odPairs = readDemands();

    cout<<"---Initialization----"<<endl;
    for (auto odPair: odPairs) {
        ShortestPath shortestPath = dijkstra(graph, odPair.getFrom(), odPair.getTo());
        for (int i=0; i<shortestPath.path.size()-1; i++) { // -1 because we have i+1 in the loop
            graph.updateInitialFlows(shortestPath.path.at(i), shortestPath.path.at(i+1), odPair.getDemand());
        }
    }
    graph.updateEdgesCost();
    graph.printAllEdges();
    //float AEC = graph.calculateAEC(odPairs);

    float AEC = INT_MAX;
    int iteration =1;
     //Iteration Loop
    while (AEC > 0.1) {
        cout<<endl<<"---Iteration "<<iteration<< "----"<<endl;
        graph.resetEdgeAuxFlow();
        //loop that finds shortest path for all OD pairs and stores them in a variable
        for (auto & odPair : odPairs) {
            ShortestPath shortestPath = dijkstra(graph, odPair.getFrom(), odPair.getTo());
            odPair.setShortestPath(shortestPath); //
        }

        AEC = graph.calculateAEC(odPairs);
        cout<<"AEC: "<< AEC<< endl;

        //loop to update auxflow at each iteration and for all od pairs
        for (auto & odPair:odPairs) {
            for (int i=0; i<odPair.getShortestPath().path.size()-1; i++) {
                graph.updateEdgeAuxFlow(odPair.getShortestPath().path.at(i), odPair.getShortestPath().path.at(i+1), odPair.getDemand());
            }
        }
        graph.updateEdgeFlow(iteration);  //for the whole graph
        graph.updateEdgesCost();           //for the whole
        graph.printAllEdges();
        iteration++;
    }

    cout<<"Best AEC: "<< AEC<< endl;

};

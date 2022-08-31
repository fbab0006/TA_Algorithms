//
// Created by fbab0006 on 5/08/2022.
//

#ifndef TA_ALGORITHMS_GRAPH_H
#define TA_ALGORITHMS_GRAPH_H

#include<iostream>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include "Edge.h"
#include "Vertex.h"
#include "TripMatrix.h"

class Graph {

    vector < Vertex > vertices;
   // vector<unsigned> start_index;
    //vector<Edge> edge_list;

public:


    void addVertex(Vertex newVertex) {
            vertices.push_back(newVertex);
//            cout << "New Vertex Added Successfully" << endl;
        }
    int getNoOfVertices() {
        return vertices.size();
    }

    Vertex getVertexByID(int vid) {
        Vertex temp = vertices[vid-1];
                return temp;
            }

    void addEdge(int fromVertex, int toVertex, double capacity, double length, double fft, double b, double power,double speed, bool toll, bool link_type) {
        Edge edge(fromVertex, toVertex, capacity, length, fft, b, power, speed, toll, link_type);
        vertices[fromVertex-1].edgeList.push_back(edge);
    }

    void updateEdgeFlowAndCost(double lambda) {
        for (int i = 0; i < vertices.size(); i++) {
            for (auto & edge : vertices.at(i).edgeList) {
                edge.calculateFlow(lambda);
                edge.calculateBPRCost();
                edge.setAuxFlow(0.0f);
           }
        }
    }

    void assignAllOrNothing(int fromVertex, int toVertex, double demand) {
         for (auto & edge:  vertices[fromVertex-1].edgeList){
             if (edge.getDestinationVertexId() == toVertex) {
                 edge.setAuxFlow(edge.getAuxFlow() + demand);
                 break;
             }
         }
    }

    double calculateAEC(const vector<TripMatrix>& odPairs) {
        double TSTT = 0.0;//t.x
        double SPTT = 0.0;//k.d
        //calculating TSTT
        for (auto vertex: vertices) {
            for (auto edge: vertex.edgeList) {
                TSTT += edge.getFlow() * edge.getCost();
            }
        }

        for (auto odPair: odPairs) {
            SPTT += odPair.getDemand() * odPair.getShortestPath().cost;
        }

        cout<<"TSTT: "<<TSTT<<endl;
        return (TSTT / SPTT) - 1;
    }

    void printAllEdges() {
        vector<Edge> edges;
        cout<<"From  To    Flow    Cost"<<endl;
        for (auto vertex: vertices) {
            for (auto edge : vertex.edgeList) {
                edges.push_back(edge);
            }
        }
       // sort(edges.begin(), edges.end(), Edge::compareEdge);
        for (auto edge : edges) {
            cout<<edge.getSourceVertexId()<<"    "<<edge.getDestinationVertexId()<<"   "<< "   "<<edge.getFlow()<< "  "<<edge.getCost() <<endl;
        }
    }

    void printGraph() {
        for (int i = 0; i < vertices.size(); i++) {
            Vertex temp;
            temp = vertices.at(i);
            cout << " (" << temp.getID() << ") --> ";
            temp.printEdgeList();
        }
    }

    double bisection() {
        double temp = 0;
        double lambda = 0.5;
        for (int i = 0; i < vertices.size(); i++) {
            for (auto &edge: vertices.at(i).edgeList) {
                double flow = (lambda * edge.getAuxFlow()) + ((1 - lambda) * edge.getFlow());
                double cost = edge.bvalue(flow);
                temp += ((edge.getAuxFlow() - edge.getFlow())  * cost);
            }
        }
        if (temp < 0) {
            temp = 0;
        } else if(temp > 1){
            temp = 1;
        }
        return temp;
    }

    double bisectionold() {
        double l = 0, u = 1, lambda;
        while (u-l > 0.0001) {
            double temp = 0;
            lambda = (l + u) / 2;
            for (int i = 0; i < vertices.size(); i++) {
                for (auto &edge: vertices.at(i).edgeList) {
                    double temp1 = ((lambda * edge.getAuxFlow()) + ((1 - lambda) * edge.getFlow()));
                    double temp2 = (edge.getAuxFlow() - edge.getFlow());
                   // double temp3 = edge.getFFT() + (temp1 / 100);
                    temp += temp2 * edge.bvalue(temp1);
                }
            }
            if (temp > 0) {
                u = lambda;
            } else {
                l = lambda;
            }
        }
        return lambda;
    }
};

Graph readGraphFromFile() {
    FILE *f = fopen("graph input.txt", "r");
    if (f == NULL) {
        cout<<"Cannot Open file";
        exit(EXIT_FAILURE);
    }
    int V;
    if (fscanf(f, "%d", &V) != 1) {
        exit(EXIT_FAILURE);
    }

    int N;
    if (fscanf(f, "%d", &N) != 1) {
        exit(EXIT_FAILURE);
    }
    Graph graph;
    Vertex v1;
    for (int i = 0; i < V; ++i){
        v1.setID(i+1);
        graph.addVertex(v1);
    }
    for (int i = 0; i < N; ++i) {
        // Read a list of vertices and dump it on stdout with commas
        int scanResponse;
        double numbers[10];
        int counter = 0;
        do {
            double number;

            fscanf(f, "%lf%*[ ]", &number);
            numbers[counter++] = number;

            char newline[2];
            scanResponse = fscanf(f, "%[;]", &newline);
        } while (scanResponse == 0);

        graph.addEdge( numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5], numbers[6],numbers[7], numbers[8], numbers[9]);

        if (scanResponse == -1 || scanResponse == 0) {
            break;
        }
    }

    fclose(f);

    return graph;
}

#endif //TA_ALGORITHMS_GRAPH_H

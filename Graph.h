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

public:

    bool checkIfVertexExistByID(int vid) {
        bool flag = false;
        for (int i = 0; i < vertices.size(); i++) {
            if (vertices.at(i).getID() == vid) {
                return true;
            }
        }
        return flag;
    }

    void addVertex(Vertex newVertex) {
        bool check = checkIfVertexExistByID(newVertex.getID());
        if (check == true) {
//            cout << "Vertex with this ID already exist" << endl;
        } else {
            vertices.push_back(newVertex);
//            cout << "New Vertex Added Successfully" << endl;
        }
    }

    Vertex getVertexByID(int vid) {
        Vertex temp;
        for (int i = 0; i < vertices.size(); i++) {
            temp = vertices.at(i);
            if (temp.getID() == vid) {
                return temp;
            }
        }
        return temp;
    }

    bool checkIfEdgeExistByID(int fromVertex, int toVertex) {
        Vertex v = getVertexByID(fromVertex);
        list < Edge > e;
        e = v.getEdgeList();
        bool flag = false;
        for (auto it = e.begin(); it != e.end(); it++) {
            if (it -> getDestinationVertexId() == toVertex) {
                flag = true;
                return flag;
            }

        }
        return flag;
    }

    void addEdgeByID(int id, int fromVertex, int toVertex, double capacity, double length, double fft, double b, double power,double speed, bool toll, bool link_type) {
        bool check1 = checkIfVertexExistByID(fromVertex);
        bool check2 = checkIfVertexExistByID(toVertex);

        bool check3 = checkIfEdgeExistByID(fromVertex, toVertex);
        if ((check1 && check2)) {

            if (check3) {
//                cout << "Edge between " << getVertexByID(fromVertex).getID() << "(" << fromVertex << ") and " << getVertexByID(toVertex).getID() << "(" << toVertex << ") Already Exist" << endl;
            } else {

                for (auto & vertice : vertices) {
                    if (vertice.getID() == fromVertex) {
                        Edge edge(id, fromVertex, toVertex, capacity, length, fft, b, power,speed, toll, link_type);
                        vertice.edgeList.push_back(edge);
                    }
                }
//                cout << "Edge between " << fromVertex << " and " << toVertex << " added Successfully" << endl;
            }
        } else {
//            cout << "Invalid Vertex ID entered.";
        }
    }

    void assignAllOrNothing(int fromVertex, int toVertex, double demand) {
        for (auto & vertex : vertices) {  //going through all the vertices in the graph
            if (vertex.getID() == fromVertex) {
                for (auto & edge: vertex.edgeList) {
                    if (edge.getDestinationVertexId() == toVertex) {
                        edge.setAuxFlow(edge.getAuxFlow() + demand);
                        break;
                    }
                }
                break;
            }
        }
    }
   //line 18 & 19 of pseudocode
    void updateEdgeAuxFlow(int fromVertex, int toVertex, double odPair) {
            for (auto & vertex : vertices) {
                if (vertex.getID() == fromVertex) {
                    for (auto & edge: vertex.edgeList) {
                        if (edge.getDestinationVertexId() == toVertex) {
                            edge.setAuxFlow(edge.getAuxFlow() + odPair); //how
                            break;
                        }
                    }
                    break;
                }
            }
    }

    void updateEdgeFlowAndCost(double lambda) {
        for (int i = 0; i < vertices.size(); i++) {
            for (auto & edge : vertices.at(i).edgeList) {
                edge.calculateFlow(lambda);
                edge.calculateBPRCost();
            }
        }
    }

    void resetEdgeAuxFlow() {
        for (int i = 0; i < vertices.size(); i++) {
            for (auto edge = vertices.at(i).edgeList.begin(); edge != vertices.at(i).edgeList.end(); edge++) {
                edge->setAuxFlow(0.0f);
            }
        }
    }

 /*  void updateEdgesCost() {
        for (int i = 0; i < vertices.size(); i++) {
            for (auto & edge : vertices.at(i).edgeList) {
                edge.calculateBPRCost();
                //cout<< edge.getFlow()<<"AEC: "<< edge.getCost()<< endl;
            }
        }
    }
    */

    double calculateAEC(const vector<TripMatrix>& odPairs) {
        double sumTemp1 = 0.0;//t.x
        double sumTemp2 = 0.0;//k.d
        double sumDemand = 0.0;//d.1
        //calculating TSTT
        for (auto vertex: vertices) {
            for (auto edge: vertex.edgeList) {
                sumTemp1 += edge.getFlow() * edge.getCost();
            }
        }

        for (auto odPair: odPairs) {
            sumTemp2 += odPair.getDemand() * odPair.getShortestPath().cost;
            sumDemand += odPair.getDemand();
        }
        cout<<"TSTT: "<<sumTemp1<<" SPTT: "<<sumTemp2<<" D: "<<sumDemand<<endl;
        cout<<"Diff: "<<(sumTemp1 - sumTemp2)<<" AEC: "<<(sumTemp1 - sumTemp2)/sumDemand<<endl;
        return (sumTemp1 - sumTemp2)/sumDemand;
    }

    void printAllEdges() {
        vector<Edge> edges;
        cout<<"From  To    Flow    Cost"<<endl;
        for (auto vertex: vertices) {
            for (auto edge : vertex.edgeList) {
                edges.push_back(edge);
            }
        }
        sort(edges.begin(), edges.end(), Edge::compareEdge);
        for (auto edge : edges) {
            cout<<edge.getSourceVertexId()<<"    "<<edge.getDestinationVertexId()<<"   "<< "   "<<edge.getFlow()<< "  "<<edge.getCost() <<endl;
        }
    }

    int getNoOfVertices() {
        return vertices.size();
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
        double l = 0, u = 1, lambda = 0;
        int i= 0;
        while (i<5) {
            double temp = 0;
            for (int i = 0; i < vertices.size(); i++) {
                for (auto &edge: vertices.at(i).edgeList) {
                    lambda = (l + u) / 2;
                    double temp1 = ((lambda * edge.getAuxFlow()) + ((1 - lambda) * edge.getFlow()));
                    double temp2 = (edge.getAuxFlow() - edge.getFlow());
                    double temp3 = edge.getFFT() + (temp1 / 100);
                    temp += temp2 * temp3;
                }
            }
            if (temp > 0) {
                u = lambda;
            } else{
                l = lambda;}
            i++;
        }
        lambda = (u+l)/2;
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
    for (int i = 0; i < N; ++i) {
        // Read a list of vertices and dump it on stdout with commas
        int scanResponse;
        double numbers[11];
        int counter = 0;
        do {
            double number;

            fscanf(f, "%lf%*[ ]", &number);
            numbers[counter++] = number;

            char newline[2];
            scanResponse = fscanf(f, "%[;]", &newline);
        } while (scanResponse == 0);

        v1.setID(numbers[1]);
        graph.addVertex(v1);
        v1.setID(numbers[2]);
        graph.addVertex(v1);
        graph.addEdgeByID( numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5], numbers[6],numbers[7], numbers[8], numbers[9], numbers[10]);

        if (scanResponse == -1 || scanResponse == 0) {
            break;
        }
    }

    fclose(f);

    return graph;
}

#endif //TA_ALGORITHMS_GRAPH_H

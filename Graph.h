//
// Created by fbab0006 on 5/08/2022.
//

#ifndef TA_ALGORITHMS_GRAPH_H
#define TA_ALGORITHMS_GRAPH_H

#include<iostream>
#include <vector>
#include <list>
#include <iterator>
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

    void addEdgeByID(int id, int fromVertex, int toVertex, float fft) {
        bool check1 = checkIfVertexExistByID(fromVertex);
        bool check2 = checkIfVertexExistByID(toVertex);

        bool check3 = checkIfEdgeExistByID(fromVertex, toVertex);
        if ((check1 && check2)) {

            if (check3) {
//                cout << "Edge between " << getVertexByID(fromVertex).getID() << "(" << fromVertex << ") and " << getVertexByID(toVertex).getID() << "(" << toVertex << ") Already Exist" << endl;
            } else {

                for (auto & vertice : vertices) {
                    if (vertice.getID() == fromVertex) {
                        Edge edge(id, toVertex, fft);
                        vertice.edgeList.push_back(edge);
                    }
                }
//                cout << "Edge between " << fromVertex << " and " << toVertex << " added Successfully" << endl;
            }
        } else {
//            cout << "Invalid Vertex ID entered.";
        }
    }

    void updateInitialFlows(int fromVertex, int toVertex, float odPair) {
        bool check = checkIfEdgeExistByID(fromVertex, toVertex);
        if (check) {
            for (auto & vertex : vertices) {  //going through all the vertices in the graph
                if (vertex.getID() == fromVertex) {
                    for (auto edge = vertex.edgeList.begin(); edge != vertex.edgeList.end(); edge++) {
                        if (edge -> getDestinationVertexId() == toVertex) {
                            edge->setFlow(odPair);
                            edge->setAuxFlow(odPair);
                            break;
                        }
                    }
                    break;
                }
            }
//            cout << "Edge Weight Updated Successfully " << endl;
        } else {
            cout << "Edge between " << getVertexByID(fromVertex).getID() << "(" << fromVertex << ") and " << getVertexByID(toVertex).getID() << "(" << toVertex << ") DOES NOT Exist" << endl;
        }
    }
   //line 18 & 19 of pseudocode
    void updateEdgeAuxFlow(int fromVertex, int toVertex, float odPair) {
        bool check = checkIfEdgeExistByID(fromVertex, toVertex);
        if (check) {
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
//            cout << "Edge Weight Updated Successfully " << endl;
        } else {
            cout << "Edge between " << getVertexByID(fromVertex).getID() << "(" << fromVertex << ") and " << getVertexByID(toVertex).getID() << "(" << toVertex << ") DOES NOT Exist" << endl;
        }
    }

    void updateEdgeFlow(int iteration) {
        for (int i = 0; i < vertices.size(); i++) {
            for (auto & edge : vertices.at(i).edgeList) {
                edge.calculateFlow(iteration);
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

    void updateEdgesCost() {
        for (int i = 0; i < vertices.size(); i++) {
            for (auto edge = vertices.at(i).edgeList.begin(); edge != vertices.at(i).edgeList.end(); edge++) {
                edge->calculateCost();
            }
        }
    }

    float calculateAEC(const vector<TripMatrix>& odPairs) {
        float sumTemp1 = 0.0f;//t.x
        float sumTemp2 = 0.0f;//k.d
        float sumDemand = 0.0f;//d.1
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

        return (sumTemp1 - sumTemp2)/sumDemand;
    }

    void printAllEdges() {
        cout<<"Id   Cost    Flow"<<endl;
        for (auto vertex: vertices) {
            for (auto edge : vertex.edgeList) {
                cout<<edge.getId()<<"   "<<edge.getCost()<< "   "<<edge.getFlow()<<endl;
            }
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
        float numbers[4];
        int counter = 0;
        do {
            int number;
            scanResponse = fscanf(f, "%d%*[ ]", &number);
            if (scanResponse != 1) {
                break;
            }
            numbers[counter++] = number;

            char newline[2];
            scanResponse = fscanf(f, "%[\n]", newline);
        } while (scanResponse == 0);

        v1.setID(numbers[1]);
        graph.addVertex(v1);
        v1.setID(numbers[2]);
        graph.addVertex(v1);
        graph.addEdgeByID(numbers[0], numbers[1], numbers[2], numbers[3]);

        if (scanResponse == -1 || scanResponse == 0) {
            break;
        }

    }

    fclose(f);

    return graph;
}

#endif //TA_ALGORITHMS_GRAPH_H

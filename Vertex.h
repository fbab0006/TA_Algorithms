//
// Created by fbab0006 on 5/08/2022.
//

#ifndef TA_ALGORITHMS_VERTEX_H
#define TA_ALGORITHMS_VERTEX_H
//#include "Edge.h"

using namespace std;

class Vertex {
public:
    int vid;
    list <Edge> edgeList;

    Vertex() {
        vid = 0;
    }

    Vertex(int id, string sname) {
        id = id;
    }
    int getID() {
        return vid;
    }
    void setID(int id) {
        vid = id;
    }
    list <Edge> getEdgeList() {
        return edgeList;
    }
    void printEdgeList() {
        cout << "[";
        for (auto it = edgeList.begin(); it != edgeList.end(); it++) {
            cout << it -> getDestinationVertexId() << "(" << it -> getCost() << ") --> ";
        }
        cout << "]";
        cout << endl;
    }

};

#endif //TA_ALGORITHMS_VERTEX_H

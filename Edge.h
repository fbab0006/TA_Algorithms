//
// Created by fbab0006 on 5/08/2022.
//
//
// Created by fbab0006 on 5/08/2022.
//

#ifndef TA_ALGORITHMS_EDGE_H
#define TA_ALGORITHMS_EDGE_H
#include <cmath>


class Edge {
    //int id;
    int sourceVertexId;
    int destinationVertexId;
    double flow;//X
    double auxflow;//X*
    double cost;//t
    double fft;//initial weight
    double capacity;
    double beta;   //b
    double alpha; //power
    double length;
    double speed;
    bool toll;
    bool link_type;


public:
    Edge(int fromVertex, int toVertex, double capacity, double length, double fft, double b, double power,double speed, bool toll, bool link_type) {
        Edge::sourceVertexId = fromVertex;
        Edge::destinationVertexId = toVertex;
        Edge::fft = fft;
        Edge::flow = 0.0;
        Edge::auxflow = 0.0;
        Edge::cost = fft;
        Edge::capacity = capacity;
        Edge::length= length;
        Edge::alpha = b;
        Edge::beta = power;
        Edge::speed = speed;
        Edge::toll = toll;
        Edge::link_type = link_type;
    }
  //  int getId() {
    //    return id;
    //}
    double getAlpha() {
        return alpha;
    }

    int getSourceVertexId() {
        return sourceVertexId;
    }

    void setSourceVertexId(int sourceVertexId) {
        Edge::sourceVertexId = sourceVertexId;
    }

    double getCapacity() {
        return capacity;
    }

    double getBeta() {
        return beta;
    }


    double getFFT() {
        return fft;
    }

    double getCost()  {
        return cost;
    }

    void setCost(double cost) {
        Edge::cost = cost;
    }

    int getDestinationVertexId() {
        return destinationVertexId;
    }

    void setFlow(double flow) {
        Edge::flow = flow;
    }

    double getFlow() {
        return flow;
    }

    void setAuxFlow(double auxflow) {
        Edge::auxflow = auxflow;
    }

    double getAuxFlow() {
        return auxflow;
    }

    void calculateFlow(double lambda) {
        flow = (lambda * auxflow) + ((1 - lambda) * flow);
    }

    void calculateBPRCost(){
        double temp = (1.0 + (alpha * pow((flow/capacity), beta)));
        cost = fft * temp;
    }
   double bvalue(double f){
       double temp = fft*(1.0 + (alpha * pow((f/capacity), beta)));
       return temp;
    };

  /*  static bool compareEdge(Edge e1, Edge e2)
    {
        return (e1.id < e2.id);
    }*/
};
#endif //TA_ALGORITHMS_EDGE_H

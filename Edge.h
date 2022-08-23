//
// Created by fbab0006 on 5/08/2022.
//

#ifndef TA_ALGORITHMS_EDGE_H
#define TA_ALGORITHMS_EDGE_H
#include <cmath>


class Edge {
    int id;
    int destinationVertexId;
    float flow;//X
    float auxflow;//X*
    float cost;//t
    float fft;//initial weight
    float capacity;
    float beta;
    float alpha;


public:


    Edge(int id, int destinationVertexId, float fft) {
        Edge::id = id;
        Edge::destinationVertexId = destinationVertexId;
        Edge::fft = fft;
        Edge::flow = 0;
        Edge::auxflow = 0;
        Edge::cost = fft;
    }

    Edge(float flow, float auxflow) : flow(flow), auxflow(auxflow) {}

    float getId() {
        return id;
    }
    float getFFT() {
        return fft;
    }

    float getCost() const {
        return cost;
    }

    void setCost(float cost) {
        Edge::cost = cost;
    }

    int getDestinationVertexId() {
        return destinationVertexId;
    }

    void setFlow(float flow) {
        Edge::flow = flow;
    }

    float getFlow() {
        return flow;
    }

    void setAuxFlow(float auxflow) {
        Edge::auxflow = auxflow;
    }

    float getAuxFlow() {
        return auxflow;
    }

    void calculateCost(){
        Edge::cost = fft + (flow/100);
    }

    void calculateFlow(int iteration) {
        float lambda = 1.0f / (iteration + 1.0f);
        flow = (lambda * auxflow) + ((1 - lambda) * flow);
    }

//    double CostFunction(float alpha, float beta, float capacity, float flow){
//        double cost = fft * (1 + alpha * pow((flow * 1.0 / capacity), beta));
//        return cost;
//
//    }
};
#endif //TA_ALGORITHMS_EDGE_H

//
// Created by fbab0006 on 6/08/2022.
//

#ifndef TA_ALGORITHMS_TRIPMATRIX_H
#define TA_ALGORITHMS_TRIPMATRIX_H

#include <vector>
#include <cstdio>
#include <iostream>
#include "ShortestPath.h"

using namespace std;

class TripMatrix{
  int from;
  int to;
  double demand;
  ShortestPath shortestPath;
public:
    TripMatrix(int from, int to, double demand) : from(from), to(to), demand(demand) {}

    int getFrom() const {
        return from;
    }

    int getTo() const {
        return to;
    }

    double getDemand() const {
        return demand;
    }

    ShortestPath getShortestPath() {
        return shortestPath;
    }

    void setShortestPath(ShortestPath shortestPath) {
        TripMatrix::shortestPath = shortestPath;
    }
};

vector<TripMatrix> readTripMatrix() {
    FILE *f = fopen("OD_pairs.txt", "r");
    if (f == NULL) {
        cout<<"Cannot Open file";
        exit(EXIT_FAILURE);
    }
    int Z;
    if (fscanf(f, "%d", &Z)==-1) {
        exit(EXIT_FAILURE);
    }

    vector < TripMatrix > odPairs;
    for (int i = 0; i < Z; ++i) {
        // Read a list of vertices id, create them and add them to a graph
        int scanResponse;
        double numbers[3];
        do {
            for (int j = 0; j < 3; ++j) {
                double number;
                scanResponse = fscanf(f, "%lf%*[ ]", &number);
                if (scanResponse != 1) {
                    break;
                }
                numbers[j] = number;
            }
            char newline[2];
            scanResponse = fscanf(f, "%[\n]", newline);
        } while (scanResponse == 0);

        TripMatrix demand(numbers[0], numbers[1], numbers[2]);
        odPairs.push_back(demand);
        if (scanResponse == -1 || scanResponse == 0) {
            break;
        }
    }

    fclose(f);
    return odPairs;
}

#endif //TA_ALGORITHMS_TRIPMATRIX_H

//
// Created by fbab0006 on 6/08/2022.
//

#ifndef TA_ALGORITHMS_ODPAIR_H
#define TA_ALGORITHMS_ODPAIR_H

#include <vector>
#include <cstdio>
#include <iostream>
#include "ShortestPath.h"

using namespace std;

class ODPair{
  int from;
  int to;
  float odPair;
  ShortestPath shortestPath;
public:
    ODPair(int from, int to, float odPair) : from(from), to(to), odPair(odPair) {}

    int getFrom() const {
        return from;
    }

    int getTo() const {
        return to;
    }

    float getDemand() const {
        return odPair;
    }

    ShortestPath getShortestPath() {
        return shortestPath;
    }

    void setShortestPath(ShortestPath shortestPath) {
        ODPair::shortestPath = shortestPath;
    }
};

vector<ODPair> readDemands() {
    FILE *f = fopen("OD_pairs.txt", "r");
    if (f == NULL) {
        cout<<"Cannot Open file";
        exit(EXIT_FAILURE);
    }
    int Z;
    if (fscanf(f, "%d", &Z)==-1) {
        exit(EXIT_FAILURE);
    }

    vector < ODPair > odPairs;
    for (int i = 0; i < Z; ++i) {
        // Read a list of vertices id, create them and add them to a graph
        int scanResponse;
        int numbers[3];
        int counter = 0;
        do {
            for (int j = 0; j < 3; ++j) {
                int number;
                scanResponse = fscanf(f, "%d%*[ ]", &number);
                if (scanResponse != 1) {
                    break;
                }
                //numbers[counter++] = number;
                numbers[j] = number;
            }
            char newline[2];
            scanResponse = fscanf(f, "%[\n]", newline);
        } while (scanResponse == 0);

        ODPair odPair(numbers[0], numbers[1], numbers[2]);
        odPairs.push_back(odPair);

        if (scanResponse == -1 || scanResponse == 0) {
            break;
        }
    }

    fclose(f);
    return odPairs;
}

#endif //TA_ALGORITHMS_ODPAIR_H

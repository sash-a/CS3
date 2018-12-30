#include <iostream>
#include "KMeans.h"


using namespace std;

int main()
{
    vector<dataPoint> initialCentroids = {dataPoint(1, 2, 10), dataPoint(4, 5, 8), dataPoint(7, 1, 2)};
    KMeans km("data.txt", initialCentroids, 3);

    string file = "results.txt";
    int it = 0;

    km.clearFile(file);
    km.learn(it, initialCentroids, file);

}
//
// Created by abrsas002 on 2018/08/01.
//

#ifndef KMEANS_KMEANS_H
#define KMEANS_KMEANS_H

#include <vector>
#include <string>
#include <iostream>

struct dataPoint
{
    float x;
    float y;
    int id;

    dataPoint(int id, float x, float y);
};

class KMeans
{
public:
    std::vector<dataPoint> data;
    std::vector<dataPoint> centroids;
    int numCentroids;

    // Entries in the same order centroids are in i.e item at clusters[0][0] has centroid at centroids[0]
    std::vector<std::vector<dataPoint>> clusters;


    KMeans(const std::string &file, const std::vector<dataPoint> &centroids, const int &numCentroids)
    {
        data = readData(file);
        this->centroids = centroids;
        this->numCentroids = numCentroids;

        for (int i = 0; i < numCentroids; ++i)
        {
            std::vector<dataPoint> cluster;
            clusters.push_back(cluster);
            clusters[i].reserve(10);
        }
    }

    std::vector<dataPoint> readData(const std::string &);

    // returns the index of the centroid in the centroids list
    int findNearestCentroid(const dataPoint &);

    void addToCluster(const dataPoint &, const int &);

    void findNewCentroids();

    void learn(int &iteration, const std::vector<dataPoint> prevCentroids, const std::string &file);

    void writeResults(const std::string &, const int &);

    void clearFile(const std::string &);
};


#endif //KMEANS_KMEANS_H

//
// Created by abrsas002 on 2018/08/01.
//

#include "KMeans.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

std::vector<dataPoint> KMeans::readData(const std::string &file)
{
    std::vector<dataPoint> d;
    std::ifstream in(file);

    std::string line;

    while (getline(in, line))
    {
        std::vector<int> vals;

        std::stringstream splitter(line);
        std::string val;

        for (int i = 0; i < 3; ++i)
        {
            getline(splitter, val, ',');
            vals.push_back(std::stoi(val));
        }
        dataPoint point(vals[0], vals[1], vals[2]);


        d.push_back(point);
    }
    in.close();

    return d;

}

int KMeans::findNearestCentroid(const dataPoint &point)
{
    std::vector<float> dists;
    for (auto &centroid : centroids)
    {
        float dist = sqrt(pow(centroid.x - point.x, 2) + pow(centroid.y - point.y, 2));
        dists.push_back(dist);
    }

    int clusterIndex = 0;
    float dist = dists[0];
    for (int j = 1; j < dists.size(); ++j)
    {
        if (dists[j] < dist)
        {
            clusterIndex = j;
            dist = dists[j];
        }
    }

    addToCluster(point, clusterIndex);
    return clusterIndex;
}

void KMeans::addToCluster(const dataPoint &point, const int &clusterID)
{
    clusters[clusterID].push_back(point);
}

void KMeans::findNewCentroids()
{
    int pos = 0;
    for (auto &cluster : clusters)
    {
        float avgx = 0;
        float avgy = 0;
        for (auto &point: cluster)
        {
            avgx += point.x;
            avgy += point.y;
        }

        avgx /= cluster.size();
        avgy /= cluster.size();

        centroids[pos] = dataPoint(-pos - 1, avgx, avgy);

        cluster.clear();
        ++pos;
    }
}

void KMeans::learn(int &iteration, const std::vector<dataPoint> prevCentroids, const std::string &file)
{
    for (auto &point : data) findNearestCentroid(point);

    writeResults(file, iteration);

    findNewCentroids();
    for (int i = 0; i < centroids.size(); ++i)
    {
        if (centroids[i].x != prevCentroids[i].x || centroids[i].y != prevCentroids[i].y)
        {
            learn(++iteration, centroids, file);
            break;
        }
    }
}

void KMeans::writeResults(const std::string &file, const int &iteration)
{
    std::ofstream out(file, std::ios_base::app);

    out << "Iteration: " << iteration << "\n\n";

    for (int i = 0; i < numCentroids; ++i)
    {
        out << "Cluster " << i << ": ";
        for (auto &j : clusters[i])
        {
            out << j.id << " ";
        }
        out << "\n" << "Centroid: (" << centroids[i].x << ", " << centroids[i].y << ")";

        out << "\n\n";
    }

    out << "\n";
    out.close();
}

void KMeans::clearFile(const std::string &file)
{
    std::ofstream ofs;
    ofs.open(file, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}


dataPoint::dataPoint(int id, float x, float y) : id(id), x(x), y(y)
{}

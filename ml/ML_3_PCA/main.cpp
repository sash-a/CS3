#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>

#include "Eigen/Eigen/Eigenvalues"
#include "Eigen/Eigen/Core"

using namespace std;
using namespace Eigen;

const int dataSize = 64;
typedef Eigen::Matrix<float, dataSize, 2> Matrixx;
typedef Eigen::Matrix<float, 2, 2> Mat2x2;

// struct Rainfall
// {
// 	string place;
//     float jan;
//     float jul;

//     Rainfall(string p, float ja, float ju) : place(p), jan(ja), jul(ju) {}
// };

void read(const string &filename, vector<string> &placeNames, Matrixx & data);
float getAvg(Matrixx &m, const int &col);
float covariance(const float &avgX, const float &avgY, const Matrixx &data);
float variance(const int &col, const int &avg, const Matrixx &data);


void read(const string &filename, vector<string> &placeNames, Matrixx & data)
{
	/*
	File structured:

	Place
	Jan
	Jul
	Place
	Jan
	Jul
	...
	*/

	ifstream in(filename);
	string line;
	// Skipping first 3 lines
	for (int _ = 0; _ < 3; ++_) { getline(in, line); }

	int row = 0;
	while (getline(in, line))
	{
		placeNames.push_back(line);
	
		getline(in, line);
		data(row, 0) = stof(line);

		getline(in, line);
		data(row, 1) = stof(line);
		
		++row;
	}
}

float getAvg(const int &col, Matrixx &m)
{
	float sum = 0;
	for (int i = 0; i < dataSize; ++i)
	{
		sum += m(i, col);
	}

	return sum/dataSize;
}

float covariance(const float &avgX, const float &avgY, const Matrixx &data)
{
	float sum = 0;

	for (int i = 0; i < dataSize; ++i)
	{
		float varX = data(i, 0) - avgX;
		float varY = data(i, 1) - avgY;

	
		sum += varX * varY;
	}

	return sum / (dataSize - 1);
}

float variance(const int &col, const float &avg, const Matrixx &data)
{
	float sum = 0;
	for (int i = 0; i < dataSize; ++i)
	{
		sum += pow(data(i, col) - avg, 2);
	}

	return sum / (dataSize - 1);
}

int main()
{ 
	string filename = "data.txt";

	vector<string> placeNames;
	Matrixx data;

	read(filename, placeNames, data);

	Mat2x2 covarMat;
	float avgX = getAvg(0, data);
	float avgY = getAvg(1, data);

	cout << "Avg: " << avgX << ", " << avgY << endl;

	covarMat(0, 0) = variance(0, avgX, data); // variance x
	covarMat(0, 1) = covariance(avgX, avgY, data); // covar x and y
	covarMat(1, 1) = variance(1, avgY, data); // variance y
	covarMat(1, 0) = covariance(avgX, avgY, data); // covar y and x

	cout << "Covariance matrix:\n\n";
	cout << covarMat << endl << endl;

	EigenSolver<Mat2x2> eignen(covarMat);
	cout << "Values: "  << endl << eignen.eigenvalues() << endl << endl;
	cout << "Vectors: " << endl << eignen.eigenvectors() << endl << endl;
}





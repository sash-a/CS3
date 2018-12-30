#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h> 
#include <time.h> 

using namespace std;

struct Xor
{
	int var1;
	int var2;
	int out;

	Xor (int v1, int v2, int o) : var1(v1), var2(v2), out(o) {}
};

void learn(Xor &data, vector<pair<float, float>> &weights);

int main ()
{
	vector<pair<float, float>> weights;
	srand(12012102);

	for (int i = 0; i < 3; ++i)
	{
		pair <float, float> p = make_pair<float, float>
		(
			((float)rand()/ (RAND_MAX) * 2 - 1),
			((float)rand()/ (RAND_MAX) * 2 - 1)
		);
		weights.push_back(p);
		cout << p.first << " " << p.second << endl;
	}

	std::vector<Xor> data = 
	{
		Xor(1, 1, 0),
		Xor(1, 0, 1),
		Xor(0, 1, 1),
		Xor(0, 0, 0)
	};

	for (int i = 0; i < 200; ++i)
	{
		// cout << "weights before: " << weights[0].first << endl;
		learn(data[i % data.size()], weights);
		// cout << "weights after: " << weights[0].first << endl;
	}

	for(auto p : weights)
	{
		cout << p.first << " " << p.second << endl;
	}
}

void learn(Xor &data, vector<pair<float, float>> &weights)
{
	float rate = 0.02f;
	pair<float, float> hiddenLayer;

	hiddenLayer = make_pair
	(
		data.var1 * weights[0].first + data.var2 * weights[0].second + 0.7,
		data.var1 * weights[1].first + data.var2 * weights[1].second + 0.7
	);

	float output = hiddenLayer.first * weights[2].first + hiddenLayer.second * weights[2].second;
	int flattenedOut = (output <= 1.0f ? 1 : 0); // Flattening the output

	cout << data.var1 << " xor " << data.var2 << " = " << flattenedOut << "  " << output << endl;

	float error = data.out - flattenedOut;
	if (data.out - flattenedOut)
	{
		cout << "Changing " << error << endl;
	}

	// weights leading to first hidden node
	weights[0].first += rate * error * data.var1;
	weights[0].second += rate * error * data.var2;

	// weights leading to second hidden node
	weights[1].first += rate * error * data.var1;
	weights[1].second += rate * error * data.var2;

	// weights leading to output
	weights[2].first += rate * error * hiddenLayer.first;
	weights[2].second += rate * error * hiddenLayer.second;

	for(auto p : weights)
	{
		// cout << p.first << " " << p.second << endl;
	}

	// adjusting bias weights
	// weights[3].first -= rate * error
}
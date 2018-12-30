#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h> 
#include <time.h> 

using namespace std;

struct data
{
	int var1;
	int var2;
	int out;

	data (int v1, int v2, int o) : var1(v1), var2(v2), out(o) {}
};

bool learn(data &data, pair<float, float> &weights, const string &type, int & numCorrect);
void doXor(int x1, int x2, pair<float, float> weightsAnd, pair<float, float> weightsOr);

int main ()
{
	srand(2134345);
	pair<float, float> weightsAnd = make_pair<float, float>
	(
		((float)rand()/ (RAND_MAX) * 2 - 1),
		((float)rand()/ (RAND_MAX) * 2 - 1)
	);

	srand(67895);
	pair<float, float> weightsOr = make_pair<float, float>
	(
		((float)rand()/ (RAND_MAX) * 2 - 1),
		((float)rand()/ (RAND_MAX) * 2 - 1)
	);
	

	// xor training data
	std::vector<data> x = 
	{
		data(1, 1, 0),
		data(1, 0, 1),
		data(0, 1, 1),
		data(0, 0, 0)
	};

	// and training data
	std::vector<data> a = 
	{
		data(1, 1, 1),
		data(1, 0, 0),
		data(0, 1, 0),
		data(0, 0, 0)
	};

	// or training data
	std::vector<data> o = 
	{
		data(1, 1, 1),
		data(1, 0, 1),
		data(0, 1, 1),
		data(0, 0, 0)
	};

	int numCorrect = 0;
	// training and
	for (int i = 0; i < 200; ++i)
	{
		if(learn(a[i % a.size()], weightsAnd, " and ", numCorrect)) break;
	}

	cout << "And perceptron trained\n\n\n";

	numCorrect = 0;
	// training or
	for (int i = 0; i < 200; ++i)
	{
		if (learn(o[i % o.size()], weightsOr, " or ", numCorrect)) break;
	}

	// This is sufficient to get xor as or - and is xor
	cout << "Enter 1 or 0: " << endl;
	string input;
	cin >> input;
	int x1 = stoi(input);

	cout << "Enter 1 or 0: " << endl;
	input = "";
	cin >> input;
	int x2 = stoi(input);

	doXor(x1, x2, weightsAnd, weightsOr);
}

/*
	Learns binary and, and binary or function using perceptrons
	Returns true when there are 4 correct outputs in a row
*/
bool learn(data &data, pair<float, float> &weights, const string &type, int &numCorrect)
{
	float rate = 0.5f;

	float output = data.var1 * weights.first + data.var2 * weights.second;
	int flattenedOut = (output >= 1.0f ? 1 : 0); // Threshold activation function

	cout << data.var1 << type << data.var2 << " = " << flattenedOut << ". Unflattened output: " << output << endl;

	float error = data.out - flattenedOut;
	if (error)
	{
		cout << "Changing weights, error was: " << error << endl;

		weights.first += rate * error * data.var1;
		weights.second += rate * error * data.var2;

		numCorrect = 0;

		return false;
	}

	++numCorrect;
	return numCorrect == 4;
}

void doXor(int x1, int x2, pair<float, float> weightsAnd, pair<float, float> weightsOr)
{
	if (!(x1 == 1 || x2 == 1 || x1 == 0 || x2 == 0))
	{
		cout << "Numbers must be either 0 or 1" << endl;
		return;
	}

	// Calculating or and and value of given ints
	float o = (x1 * weightsOr.first + x2 * weightsOr.second);
	o = (o >= 1.0f ? 1 : 0);
	float a = (x1 * weightsAnd.first + x2 * weightsAnd.second);
	a = (a >= 1.0f ? 1 : 0);

	// calculating xor as: or - and
	float ans = o - a;

	cout << x1 << " xor " << x2 << " = " << ans << endl;
}
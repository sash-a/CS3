#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct neuron
{
	vector<float> weights;
	float value;

	neuron(const float &v, const vector<float> &w) : value(v), weights(w) {}

	void squash()
	{
		value = 1/(1 + exp(-value));
	}

	void calcValue(const vector<float> &inputs, const vector<float> &weights)
	{
		value = 0;
		if (inputs.size() != weights.size())
		{
			cout << "Cannot calculate neuron value\n";
			return;
		}

		for (int i = 0; i < inputs.size(); ++i)
		{
			value += inputs[i] * weights[i];
		}

		squash();
	}
};

int main()
{
	vector<neuron> inputs = 
	{
		neuron(1.3f, vector<float> {0.1f, -0.4f}),
		neuron(2.7f, vector<float> {0.2f, 1.0f}),
		neuron(0.8f, vector<float> {0.5f, -0.6f}),
		neuron(1.0f, vector<float> {0.1f, -0.3f}) // bias
	};

	neuron hidden1 = neuron(0, vector<float> {0.8f});
	hidden1.calcValue
	(
		vector<float> {inputs[0].value, inputs[1].value, inputs[2].value, inputs[3].value}, 
		vector<float> {inputs[0].weights[0], inputs[1].weights[0], inputs[2].weights[0], inputs[3].weights[0]}
	);

	neuron hidden2 = neuron(0, vector<float> {1.0f});
	hidden2.calcValue
	(
		vector<float> {inputs[0].value, inputs[1].value, inputs[2].value, inputs[3].value}, 
		vector<float> {inputs[0].weights[1], inputs[1].weights[1], inputs[2].weights[1], inputs[3].weights[1]}
	);

	neuron hiddenBias = neuron(1.0f, vector<float> {-0.3f});

	cout << hidden1.value << endl;
	cout << hidden2.value << endl;

	neuron output = neuron(0, vector<float> {});
	output.calcValue
	(
		vector<float> {hidden1.value, hidden2.value, hiddenBias.value},
		vector<float> {hidden1.weights[0], hidden2.weights[0], hiddenBias.weights[0]}
	);
	cout << output.value << endl;
}
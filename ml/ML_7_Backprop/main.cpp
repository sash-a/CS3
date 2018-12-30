#include <iostream>
#include <vector>
#include "neuralNetwork.h"

using namespace std;

typedef NeuralNetwork::neuron neuron;


int maxTrainingIterations = 100000;
float bias = 0.2f;
float rate = 0.3f;
float targetMSE = 0.001f;

float totalItterations = 0;


vector<vector<float>> data =
{
    {0, 0, 0},
    {0, 0, 1},
    {0, 1, 0},
    {0, 1, 1},
    {1, 0, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 1, 1}
};

void train(NeuralNetwork &nn);

vector<float> targets = {0, 1, 1, 0, 1, 0, 0, 1};

int main()
{
    vector<int> shape = {3,3,1};
    NeuralNetwork nn(shape, bias, rate);
    train(nn);

    cout << "MSE: " << nn.getMSE() << endl;
    cout << "Num iterations " << totalItterations << endl;

    cout << "\nXOR:\n";
    for(auto &d : data)
    {
        nn.setInputValues(d);       
        for (auto &neuron : nn.network[0])
        {
            if (neuron.isBias)
                continue;
            
            cout << neuron.value << " ";
        }

        for (auto output : nn.getOutputValues())
            cout << "= " << round(output) << endl;
    }
}

void train(NeuralNetwork &nn)
{
                                // First train MSE is zero because no forward pass has been done yet 
    for (int i = 0; i < maxTrainingIterations && (nn.getMSE() == 0 || nn.getMSE() > targetMSE); ++i)
    {
        nn.setInputValues(data[i % data.size()]);
        nn.forward();
        nn.back(vector<float> {targets[i % data.size()]});

        totalItterations = i;
    }

    cout << "\n\nFinished Training\n\n";
}
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H NEURALNETWORK_H

class NeuralNetwork
{
public:
    struct neuron
    {
        std::vector<float> weights;
        float value;
        float error;

        int layer;
        bool isBias;

        neuron(const float &v, std::vector<float> w, const int &l, const bool &b = false) : 
            value(v), weights(std::move(w)), layer(l), isBias(b) {}

        void squash() { value = 1 / (1 + exp(-value)); }

        void compute(const std::vector<float> &inputs, const std::vector<float> &weights)
        {
            value = 0;
            if (inputs.size() != weights.size())
            {
                std::cout << "Cannot calculate neuron value\n";
                return;
            }

            for (int i = 0; i < inputs.size(); ++i)
                value += inputs[i] * weights[i];

            squash();
        }
    };

    const static int SEED = 20102103;
    float rate;

    std::vector<std::vector<neuron>> network;


    NeuralNetwork () { srand(SEED); }

    NeuralNetwork(const std::vector<int> &shape, const float &bias, const float &learningRate) : rate(learningRate)
    {
        for (int i = 0; i < shape.size(); ++i)
        {
            if (i == shape.size() - 1)
            {
                addLayer(shape[i], 0);
                continue;
            }

            addLayer(shape[i], shape[i + 1]);
        }

        for (int i = 0; i < shape.size() - 1; ++i)
            addBiasToLayer(i, bias);

        // Printing details of network
        std::cout << "Created NN\nShape = ";
        for(float f : shape)
            std::cout << f << " ";

        std::cout << "\nLearning rate: " << rate << std::endl;
    }

    void addLayer(const int &currLayerNodes, const int &nextLayerNodes)
    {
        addLayer(std::vector<neuron>());

        int currLayer = network.size() - 1;

        for (int i = 0; i < currLayerNodes; ++i)
            addNodeToLayer(currLayer, nextLayerNodes);
    }

    void addLayer(const std::vector<neuron> &neurons) { network.push_back(neurons); }

    void addNodeToLayer(const int &layer, const int &numWeights)
    {
        std::vector<float> weights;
        for (int i = 0; i < numWeights; ++i)
            weights.push_back(getRand());

        network[layer].emplace_back(0, weights, layer);
    }

    void addBiasToLayer(const int &layer, const float &value)
    {
        if (layer == network.size() - 1)
        {
            std::cout << "Cannot add bais to output layer\n";
            return;
        }

        int numWeights = network[layer + 1].size();
        std::vector<float> weights;

        for (int i = 0; i < numWeights; ++i)
            weights.push_back(value);


        network[layer].emplace_back(1, weights, layer, true);
    }

    float getRand() { return ((float) rand() / (RAND_MAX)) * 2 - 1; }

    void forward()
    {
        for (int i = 0; i < network.size(); ++i)
            computeLayer(i);
    }

    void back(const std::vector<float> &targets)
    {
        for (int i = 0; i < getOutputNodes().size(); ++i)
            outputError(getOutputNodes()[i], targets[i]);

        for (int currLayer = network.size() - 2; currLayer >= 0; --currLayer)
            for (int currNeuron = 0; currNeuron < network[currLayer].size(); ++currNeuron)
                hiddenError(network[currLayer][currNeuron], currLayer);

        updateWeights();
    }

    void setInputValues(std::vector<float> &inputValues)
    {
        int numInputs = 0;
        for(auto &input : network[0])
            if (!input.isBias) ++numInputs;
        
        if (inputValues.size() != numInputs)
        {
            std::cout << "New inputs not same dimension as old nn\n";
            return;
        }

        for (int i = 0; i < network[0].size(); ++i)
            network[0][i].value = inputValues[i];
    }

    std::vector<float> getOutputValues()
    {
        std::vector<float> outputs;
        for(auto &neuron : getOutputNodes())
            outputs.push_back(neuron.value);

        return outputs;
    }

    std::vector<neuron> &getOutputNodes() { return network[network.size() - 1]; }

    float getMSE()
    {
        float totError = 0;
        for(auto &neuron : getOutputNodes())
            totError += pow(neuron.error, 2);

        return totError/getOutputNodes().size();
    }

private:
    // to is the index of the node that the weights are going into
    std::vector<float> gatherWeights(const int &to, const std::vector<neuron> &layer)
    {
        std::vector<float> weights;
        for (const auto &node : layer)
            weights.push_back(node.weights[to]);

        return weights;
    }

    std::vector<float> gatherValues(const std::vector<neuron> &layer)
    {
        std::vector<float> values;
        for (const auto &i : layer)
            values.push_back(i.value);

        return values;
    }


    void computeLayer(const int &layer)
    {
        if (layer <= 0) // Can't compute first layer
            return;

        std::vector<neuron> &currentLayer = network[layer];
        std::vector<neuron> prevLayer = network[layer - 1];

        std::vector<float> prevLayerValues;
        std::vector<float> prevLayerWeights;

        // Getting value vector
        prevLayerValues = gatherValues(prevLayer);

        for (int i = 0; i < currentLayer.size(); ++i)
        {
            if (currentLayer[i].isBias) // Don't compute value of a bias
                continue;

            // Getting weights
            prevLayerWeights.clear();
            prevLayerWeights = gatherWeights(i, prevLayer);

            // Compute value for that node
            currentLayer[i].compute
                    (
                            prevLayerValues,
                            prevLayerWeights
                    );
        }

    }

    float outputError(neuron &n, const float &target)
    {
        float error = n.value * (1.0f - n.value) * (target - n.value);
        n.error = error;

        return error;
    }

    float hiddenError(neuron &n, const int &layer)
    {
        float error = n.value * (1 - n.value);

        float errorFromNextLayer = 0;
        for (int i = 0; i < network[layer + 1].size(); ++i)
            errorFromNextLayer += n.weights[i] * network[layer + 1][i].error;

        error *= errorFromNextLayer;
        n.error = error;

        return error;
    }

    void updateWeight(neuron &n)
    {
        for (int i = 0; i < n.weights.size(); ++i)
            n.weights[i] += rate * network[n.layer + 1][i].error * n.value;
    }

    void updateWeights()
    {
        for (int currLayer = network.size() - 2; currLayer >= 0; --currLayer)
            for (int currNeuron = 0; currNeuron < network[currLayer].size(); ++currNeuron)
                updateWeight(network[currLayer][currNeuron]);
    }
};

#endif
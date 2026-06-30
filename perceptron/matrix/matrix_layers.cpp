#include "../../include/perceptron/perceptron.h"

Layer::Layer() noexcept: input_neurons_count_{0}, output_neurons_count_{0} {}

Layer::Layer(int input_neurons_count_, int output_neurons_count_) noexcept: 
    input_neurons_count_{input_neurons_count_}, output_neurons_count_{output_neurons_count_} 
{
    neurons_.resize(output_neurons_count_);

    for (auto& neuron : neurons_) {
        neuron.weights_.resize(input_neurons_count_);
    }
}

void Layer::setRandomWeights() noexcept {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.5, 0.5);

    for (auto& neuron : neurons_) {
        neuron.bias_ = dist(gen);
        for (auto& weight : neuron.weights_) {
            weight = dist(gen);
        }
    }
}

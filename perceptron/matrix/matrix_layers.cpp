#include "../../include/perceptron/matrix_perceptron.h"

// namespace perc {

Matrix_Layer::Matrix_Layer() noexcept: Layer() {}

Matrix_Layer::Matrix_Layer(int input_neurons_count_, int output_neurons_count_) noexcept: 
    Layer{input_neurons_count_, output_neurons_count_} 
{
    neurons_.resize(output_neurons_count_);

    for (auto& neuron : neurons_) {
        neuron.weights_.resize(input_neurons_count_);
    }
}

void Matrix_Layer::setRandomWeights() noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.5, 0.5);

    for (auto& neuron : neurons_) {
        neuron.bias_ = dist(gen);
        for (auto& weight : neuron.weights_) {
            weight = dist(gen);
        }
    }
}

// }

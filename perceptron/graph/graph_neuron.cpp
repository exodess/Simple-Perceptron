#include "../../include/perceptron/graph_perceptron.h"

namespace perc {

Graph_Neuron::Graph_Neuron() noexcept: deltas_{}, output_{}, bias_{} {}

float Graph_Neuron::sigmoidalFunc(float x) noexcept {
    return  1. / (1. + exp(-x));
}

float Graph_Neuron::setRandomWeight() noexcept {
    static std::random_device rd;
    static  std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.5, 0.5);

    return dist(gen);
}

Edge::Edge() noexcept: from_{nullptr}, to_(nullptr), weight_(0.) {}

Edge::Edge(Graph_Neuron* from, Graph_Neuron* to, float weight) noexcept: 
    from_{from}, to_(to), weight_(weight) {}

}
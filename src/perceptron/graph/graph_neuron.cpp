#include "perceptron/graph_perceptron.h"

namespace perc {

Graph_Neuron::Graph_Neuron() noexcept: Neuron() {}

Edge::Edge(Graph_Neuron* from, Graph_Neuron* to, float weight) noexcept: 
    from_{from}, to_(to), weight_(weight) {}

}
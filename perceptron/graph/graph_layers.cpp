#include "perceptron/graph_perceptron.h"

namespace perc {

Graph_Layer::Graph_Layer() noexcept: Layer() {}

Graph_Layer::Graph_Layer(int input_neurons_count_, int output_neurons_count_) noexcept: 
    Layer{input_neurons_count_, output_neurons_count_} 
{
    neurons_.resize(output_neurons_count_);
}

}

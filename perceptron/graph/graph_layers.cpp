#include "../../include/perceptron/graph_perceptron.h"

namespace perc {

Graph_Layer::Graph_Layer() noexcept: input_neurons_count_{0}, output_neurons_count_{0} {}

Graph_Layer::Graph_Layer(int input_neurons_count_, int output_neurons_count_) noexcept: 
    input_neurons_count_{input_neurons_count_}, output_neurons_count_{output_neurons_count_}
{
    neurons_.resize(output_neurons_count_);
}

}

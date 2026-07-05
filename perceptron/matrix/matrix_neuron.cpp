#include "perceptron/matrix_perceptron.h"

namespace perc {

Matrix_Neuron::Matrix_Neuron(int inputs_count) noexcept {
    weights_.resize(inputs_count);
}

Matrix_Neuron::Matrix_Neuron() noexcept: Neuron() {}


}
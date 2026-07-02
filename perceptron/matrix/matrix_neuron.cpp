#include "../../include/perceptron/matrix_perceptron.h"

namespace perc {

Matrix_Neuron::Matrix_Neuron() noexcept: deltas_{}, outputs_{}, bias_{} {}

float Matrix_Neuron::sigmoidalFunc(float x) noexcept {
    return  1. / (1. + exp(-x));
}

}
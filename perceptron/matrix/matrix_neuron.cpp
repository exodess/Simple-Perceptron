#include "perceptron/perceptron.h"
#include <cmath>

namespace perc {
    Neuron::Neuron() noexcept: deltas_{}, outputs_{}, bias_{} {}

    float Neuron::sigmoidalFunc(float x) noexcept {
        return 1. / (1. + exp(-x));
    }
}
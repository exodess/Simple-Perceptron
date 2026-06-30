#include "../../include/perceptron/perceptron.h"

Neuron::Neuron() noexcept: deltas_{}, outputs_{} {}

float Neuron::sigmoidalFunc(float x) noexcept {
    return  1. / (1. + exp(-x));
}
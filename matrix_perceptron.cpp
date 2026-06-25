#include "matrix_perceptron.h"
#include <cmath>

void Matrix_perceptron::sumFunc() {
    for (int i = 0; i < layers_count; ++i) {
        Layer& layer = layers_[i];
        for (int j = 0; j < layer.neurons_.size(); ++j) {
            Neuron& neuron_ = layer.neurons_[j];
            neuron_.outputs_ = 0.;
            for (int k = 0; k < neuron_.weights_.size(); ++k) {
                neuron_.outputs_ += normalize_input_[k] * neuron_.weights_[k];
            }
            neuron_.sigmoidalFunc(neuron_.outputs_);
        }
    }
}

void Neuron::sigmoidalFunc(double& x) {
    x = 1. / (1. + exp(-x));
}

void Matrix_perceptron::backPropagation() {

    Layer& layer = layers_[layers_count - 1];

    for (int i = 0; i < OUTPUT_SIZE; ++i) {
        for (int j = 0; j < layer.neurons_.size(); j++) {
            Neuron& neuron_ = layer.neurons_[j];
            double o = neuron_.outputs_;
            neuron_.deltas_ = (o - y_) * o * (1. - o);
        }
    }

    for (int i = layers_count - 1; i > -1; i--) {
        Layer& layer_current = layers_[i];
        Layer& layer_prev = layers_[i - 1];

        for (int j = 0; j < layer_current.output_neurons_count_; ++j) {
            double error{};

            for (int k = 0; k < layer_prev.output_neurons_count_; ++k) {
                for (int g = 0; g < layer.neurons_.size(); g++) {
                    Neuron& neuron_ = layer.neurons_[g];
                    for (int w = 0; w < neuron_.weights_.size(); ++w) {
                        error += neuron_.deltas_ * neuron_.weights_[w];
                    }
                    double o = neuron_.outputs_;
                    neuron_.deltas_ = error * o * (1. - o);
                }
            }
        }
    }

    for (int l = 0; l < layers_count; ++l) {
        Layer& layer = layers_[l];

        for (int i = 0; i < layer.output_neurons_count_; ++i) {
            for (int j = 0; j < layer.input_neurons_count_; ++j) {
                for (int k = 0; k < layer.neurons_.size(); ++k) {
                    Neuron& neuron_ = layer.neurons_[k];
                    for (int w = 0; w < neuron_.weights_.size(); ++w) {
                        neuron_.weights_[w] -= learning_rate_ * neuron_.deltas_ * normalize_input_[j];
                    }
                }
            }
        }
    }
}

int Matrix_perceptron::predict() {
    Layer& last_layer_ = layers_[layers_count - 1];
    int best_index_{};
    double max_output_{last_layer_.neurons_[0].outputs_};

    for (int i = 0; i < last_layer_.neurons_.size(); ++i) {
        Neuron& neuron_ = last_layer_.neurons_[i];
        if (neuron_.outputs_ > max_output_) {
            best_index_ = i;
            max_output_ = neuron_.outputs_;
        }
    }
    return best_index_;
}

void Matrix_perceptron::training() noexcept {
    sumFunc();
    backPropagation();
}
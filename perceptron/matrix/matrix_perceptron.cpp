#include "perceptron/matrix_perceptron.h"
#include <random>
#include <cmath>

Matrix_perceptron::Matrix_perceptron(char letter, int hidden_layer_sizes) noexcept: letter_{letter}, hidden_layers_count_{hidden_layer_sizes}, layers_count{hidden_layer_sizes + 2} {
    layers_.resize(layers_count);
    int temp_neuron_count_{};

    if (hidden_layer_sizes < MAX_HIDDEN) {
        temp_neuron_count_ = 64;
    } else {
        temp_neuron_count_ = 32;
    }

    layers_[hidden_layer_sizes + 1] = Layer{temp_neuron_count_, OUTPUT_SIZE};

    for (int i = hidden_layer_sizes; i > 0; --i) {
        layers_[i] = Layer{temp_neuron_count_ * 2, temp_neuron_count_};
        temp_neuron_count_*= 2;
    }

    layers_[0] = Layer{INPUT_SIZE, temp_neuron_count_};

    y_[letter_ - LETTER_SHIFT] = 1;
}

Matrix_perceptron::Matrix_perceptron(int number, int hidden_layer_sizes) noexcept : letter_{letter}, hidden_layers_count_{hidden_layer_sizes}, layers_count{hidden_layer_sizes + 2} {
    layers_.resize(layers_count);
    int temp_neuron_count_{};

    if (hidden_layer_sizes < MAX_HIDDEN) {
        temp_neuron_count_ = 64;
    } else {
        temp_neuron_count_ = 32;
    }

    layers_[hidden_layer_sizes + 1] = Layer{temp_neuron_count_, OUTPUT_SIZE};

    for (int i = hidden_layer_sizes; i > 0; --i) {
        layers_[i] = Layer{temp_neuron_count_ * 2, temp_neuron_count_};
        temp_neuron_count_*= 2;
    }

    layers_[0] = Layer{INPUT_SIZE, temp_neuron_count_};

    y_[number] = 1;
}

Layer::Layer(int input_neurons_count_, int output_neurons_count_) noexcept: input_neurons_count_{input_neurons_count_}, output_neurons_count_{output_neurons_count_} {
    neurons_.reserve(input_neurons_count_);
    setRandomWeights();
}

void Matrix_perceptron::sumFunc() {
    for (int i = 0; i < layers_count; ++i) {
        Layer& layer = layers_[i];

        const vector<double>& input_ = 
        (i == 0) 
        ? normalize_input_ 
        : layers_[i - 1].output_vector_; 

        layer.output_vector_.clear();
        

        for (int j = 0; j < layer.neurons_.size(); ++j) {
            Neuron& neuron_ = layer.neurons_[j];
            neuron_.outputs_ = 0.;
            for (int k = 0; k < neuron_.weights_.size(); ++k) {
                neuron_.outputs_ += input_[k] * neuron_.weights_[k];
            }
            neuron_.sigmoidalFunc(neuron_.outputs_);
            layer.output_vector_.push_back(neuron_.outputs_);
        }
    }
}

void Neuron::sigmoidalFunc(double& x) {
    x = 1. / (1. + exp(-x));
}

void Layer::setRandomWeights() noexcept {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.5, 0.5);

    for (auto& neuron : neurons_) {
        for (auto& weight : neuron.weights_) {
            weight = dist(gen);
        }
    }
}

void Matrix_perceptron::backPropagation() {

    Layer& layer = layers_.back();

    for (int i = 0; i < OUTPUT_SIZE; ++i) {
        for (int j = 0; j < layer.neurons_.size(); j++) {
            Neuron& neuron_ = layer.neurons_[j];
            double o = neuron_.outputs_;
            neuron_.deltas_ = (o - y_[i]) * o * (1. - o);
        }
    }

    for (int i = layers_count - 1; i > 0; i--) {
        Layer& layer_current = layers_[i];
        Layer& layer_next = (i == layers_count - 1) ? layer_current : layers_[i + 1];
        
        for (int g = 0; g < layer_current.neurons_.size(); ++g) {
            Neuron& neuron = layer_current.neurons_[g];
            double error = 0.0;
            
            if (i == layers_count - 1) {
                error = y_[g] - neuron.outputs_;
            } else {
                for (int n = 0; n < layer_next.neurons_.size(); ++n) {
                    error += layer_next.neurons_[n].deltas_ * neuron.weights_[n];
                }
            }
            
            neuron.deltas_ = error * neuron.outputs_ * (1.0 - neuron.outputs_);
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
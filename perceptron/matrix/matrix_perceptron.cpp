#include "../../include/perceptron/perceptron.h"
#include <random>
#include <iostream>

Neuron::Neuron() noexcept: deltas_{0.}, outputs_{0.} {}

Matrix_perceptron::Matrix_perceptron(int number, int hidden_layer_sizes) noexcept: 
    number_{number}, hidden_layers_count_{hidden_layer_sizes}, layers_count{hidden_layer_sizes + 2}
{
    layers_.resize(layers_count);
    int temp_neuron_count_{};

    if (hidden_layer_sizes < MAX_HIDDEN) {
        temp_neuron_count_ = 64;
    } else {
        temp_neuron_count_ = 32;
    }

    layers_[0] = Layer{INPUT_SIZE, temp_neuron_count_};

    for (int i = 1; i <= hidden_layer_sizes; ++i) {
        int next = (i == hidden_layer_sizes) ? OUTPUT_SIZE : temp_neuron_count_ / 2;
        layers_[i] = Layer{temp_neuron_count_, next};
        temp_neuron_count_ /= 2;
    }

    layers_[hidden_layer_sizes + 1] = Layer{temp_neuron_count_ * 2, OUTPUT_SIZE};

    y_[number] = 1;
}

void Matrix_perceptron::setHiddenLayer(int value) noexcept {
    hidden_layers_count_ = value;
}

void Matrix_perceptron::setDataInput(const vector<float>& normalize_input) noexcept {
    normalize_input_ = normalize_input;
}

Layer::Layer() noexcept: input_neurons_count_{0}, output_neurons_count_{0} {}

Layer::Layer(int input_neurons_count_, int output_neurons_count_) noexcept: 
    input_neurons_count_{input_neurons_count_}, output_neurons_count_{output_neurons_count_} 
{
    neurons_.resize(output_neurons_count_);
    for (auto& neuron : neurons_)
        neuron.weights_.resize(input_neurons_count_);
    setRandomWeights();
}

void Matrix_perceptron::sumFunc() noexcept {
    for (int i = 0; i < layers_count; ++i) {
        Layer& layer = layers_[i];

        const vector<float>& input_ = 
        (i == 0) 
        ? normalize_input_ 
        : layers_[i - 1].output_vector_; 

        layer.output_vector_.clear();
        

        for (auto& neuron_ : layer.neurons_) {
            neuron_.outputs_ = 0.;
            for (int k = 0; k < neuron_.weights_.size(); ++k) {
                neuron_.outputs_ += input_[k] * neuron_.weights_[k];
            }
            neuron_.sigmoidalFunc(neuron_.outputs_);
            layer.output_vector_.push_back(neuron_.outputs_);
        }
    }
}

void Neuron::sigmoidalFunc(float& x) noexcept {
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

void Matrix_perceptron::updateWeights() noexcept {
    for (int i = 0; i < layers_count; ++i) {
        Layer& layer = layers_[i];

        const vector<float>& input_ = (i == 0)
        ? normalize_input_
        : layers_[i - 1].output_vector_;

        for (int i = 0; i < layer.output_neurons_count_; ++i) {
            for (int j = 0; j < layer.input_neurons_count_; ++j) {
                for (auto& neuron_ : layer.neurons_) {
                    for (int w = 0; w < neuron_.weights_.size(); ++w) {
                        neuron_.weights_[w] -= learning_rate_ * neuron_.deltas_ * input_[w];
                    }
                }
            }
        }
    }
}

float Matrix_perceptron::backPropagation(float expected_[OUTPUT_SIZE]) noexcept {

    Layer& layer = layers_.back();

    //обработка выходного слоя
    for (int i = 0; i < layer.neurons_.size(); ++i) {
        Neuron& neuron_ = layer.neurons_[i];
        float o = neuron_.outputs_;
        neuron_.deltas_ = (o - expected_[i]) * o * (1. - o);
        
    }

    //обработка слоев с конца
    for (int i = layers_count - 2; i > -1; i--) {
        Layer& layer_current = layers_[i];
        Layer& layer_next = layers_[i + 1];
        
        for (int g = 0; g < layer_current.neurons_.size(); ++g) {
            Neuron& neuron = layer_current.neurons_[g];
            float error = 0.0;

            for (int n = 0; n < layer_next.neurons_.size(); ++n) {
                error += layer_next.neurons_[n].deltas_ * layer_next.neurons_[n].weights_[g];
            }
            neuron.deltas_ = error * neuron.outputs_ * (1.0 - neuron.outputs_);
        }
    }

    //обновление весов
    updateWeights();

    //считаем ошибку для конкретного примера (вектора значений), чтобы высчитать ошибку (или потерю) для эпохи
    float total_error_{};
    for (int i = 0; i < layer.neurons_.size(); ++i) {
        Neuron& neuron_ = layer.neurons_[i];
        float diff_ = neuron_.outputs_ - expected_[i];
        total_error_+= diff_ * diff_;
    }

    return total_error_;
}

int Matrix_perceptron::predict(const vector<float>& image) noexcept  {
    setDataInput(image);
    sumFunc();

    Layer& last_layer_ = layers_[layers_count - 1];
    int best_index_{};
    float max_output_{last_layer_.neurons_[0].outputs_};

    for (int i = 0; i < last_layer_.neurons_.size(); ++i) {
        Neuron& neuron_ = last_layer_.neurons_[i];
        if (neuron_.outputs_ > max_output_) {
            best_index_ = i;
            max_output_ = neuron_.outputs_;
        }
    }
    return best_index_;
}

void Matrix_perceptron::training(int epoch, std::vector<EmnistData>& EmnistData_) noexcept {
    for (int i = 0; i < epoch; ++i) {
        float epoch_loss{};

        for (auto& input : EmnistData_) {

            float y_training[OUTPUT_SIZE]{};
            y_training[input.letter()] = 1;

            setDataInput(input.data());
            sumFunc();
            epoch_loss += backPropagation(y_training);
        }
        
        std::cout << "Epoch loss is " << epoch_loss << std::endl;
    }
}
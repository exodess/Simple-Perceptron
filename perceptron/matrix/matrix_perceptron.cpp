#include "perceptron/matrix_perceptron.h"
#include <data/data.h>

namespace perc {
    MatrixPerceptron::MatrixPerceptron(int number, int hidden_layer_sizes) noexcept :
    Perceptron(number, hidden_layer_sizes) {
        int layers_count = hidden_layer_sizes + 2;
        int temp_neuron_count = (hidden_layer_sizes < MAX_HIDDEN) ? 64 : 32;

        layers_.resize(layers_count);
        layers_[layers_count - 1] = Layer{temp_neuron_count, COUNT_LETTERS};

        for (int i = layers_count - 2; i > 0; --i) {
            layers_[i] = Layer{temp_neuron_count * 2, temp_neuron_count};
            temp_neuron_count *= 2;
        }

        layers_[0] = Layer{INPUT_SIZE, temp_neuron_count};
    }

    void MatrixPerceptron::_sumFunc() noexcept {
        for (int i = 0; i < hidden_layers_count_ + 2; ++i) {
            Layer& layer = layers_[i];

            const vector<float>& input = (i == 0) ? normalize_input_ : layers_[i - 1].output_vector_;
            layer.output_vector_.resize(layer.neurons_.size());

            for (int j = 0; j < layer.neurons_.size(); ++j)
            {
                Neuron& neuron = layer.neurons_[j];
                float sum = neuron.bias_;

                for (int k = 0; k < neuron.weights_.size(); ++k) {
                    sum += input[k] * neuron.weights_[k];
                }

                neuron.outputs_ = neuron.sigmoidalFunc(sum);
                layer.output_vector_[i] = neuron.outputs_;
            }
        }
    }

    void MatrixPerceptron::_updateWeights() noexcept {
        for (int l = 0; l < hidden_layers_count_ + 2; ++l) {
            Layer& layer = layers_[l];
            const vector<float>& input = (l == 0) ? normalize_input_ : layers_[l - 1].output_vector_;

            for (auto& neuron : layer.neurons_) {
                neuron.bias_ -= LEARNING_RATE * neuron.deltas_;
                for (int w = 0; w < neuron.weights_.size(); ++w)
                    neuron.weights_[w] -= LEARNING_RATE * neuron.deltas_ * input[w];
            }
        }
    }

    float MatrixPerceptron::_backPropagation(const vector<float>& expected) noexcept {
        Layer& layer = layers_.back();

        //обработка выходного слоя
        for (int i = 0; i < layer.neurons_.size(); ++i) {
            Neuron& neuron = layer.neurons_[i];
            float o = neuron.outputs_;
            neuron.deltas_ = (o - expected[i]) * o * (1. - o);
        }

        //обработка слоев с конца
        for (int i = hidden_layers_count_; i > -1; i--) {
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
        _updateWeights();

        // Считаем ошибку для конкретного примера (вектора значений),
        // чтобы высчитать ошибку (или потерю) для эпохи
        float total_error{};
        for (int i = 0; i < layer.neurons_.size(); ++i) {
            Neuron& neuron = layer.neurons_[i];
            float diff = neuron.outputs_ - expected[i];
            total_error += diff * diff;
        }

        return total_error;
    }

    void MatrixPerceptron::Reset() noexcept {
        for (auto& layer : layers_) {
            layer.setRandomWeights();
        }
    }

    int MatrixPerceptron::Verify(const vector<float>& image) noexcept {
        normalize_input_ = image;
        _sumFunc();

        Layer& last_layer = layers_[hidden_layers_count_ + 1];
        int best_index{};
        float max_output{last_layer.neurons_[0].outputs_};

        for (int i = 0; i < last_layer.neurons_.size(); ++i) {
            Neuron& neuron = last_layer.neurons_[i];
            if (neuron.outputs_ > max_output) {
                best_index = i;
                max_output = neuron.outputs_;
            }
        }

        return best_index;
    }

    float MatrixPerceptron::Train(const std::vector<EmnistData>& dataset) noexcept {
        float epoch_loss{};

        for (auto& input : dataset) {

            vector<float> y_training;
            y_training.resize(OUTPUT_SIZE);
            y_training[input.index()] = 1;

            normalize_input_ = input.data();
            _sumFunc();
            epoch_loss += _backPropagation(y_training);
        }

        return epoch_loss / dataset.size();
    }

    void MatrixPerceptron::LoadWeights(const vector<float>& data) noexcept {
        vector<float> weights = data;
        int i{};

        for (auto& layer : layers_) {
            for (auto& neuron : layer.neurons_) {
                for (auto& weight : neuron.weights_) {
                    weight = weights[i++];
                }
            }
        }
    }

    vector<float> MatrixPerceptron::GetWeights() noexcept {
        vector<float> weights{};

        for (auto& layer : layers_) {
            for (auto& neuron : layer.neurons_) {
                for (auto& weight : neuron.weights_) {
                    weights.push_back(weight);
                }
            }
        }

        return weights;
    }
}
#include "perceptron/matrix_perceptron.h"

namespace perc {

Matrix_perceptron::Matrix_perceptron(int hidden_layer_sizes) noexcept: Perceptron(MATRIX_VIEW, hidden_layer_sizes)
{
    std::vector<int> layer_sizes;
    layer_sizes.push_back(INPUT_SIZE);

    int temp_neuron_count_ = (hidden_layer_sizes < MAX_HIDDEN) ? 64 : 32;

    // Генерируем размеры от большего к меньшему
    for (int i = hidden_layer_sizes - 1; i >= 0; --i) {
        layer_sizes.push_back(temp_neuron_count_ * (1 << i));
    }
    layer_sizes.push_back(COUNT_LETTERS);

    layers_.resize(layer_sizes.size() - 1);

    for (int i = 0; i < layers_.size(); ++i) {
        layers_[i] = Matrix_Layer{layer_sizes[i], layer_sizes[i + 1]};
    }

    for (auto& layer : layers_)
        layer.setRandomWeights();
}

void Matrix_perceptron::Reset() noexcept {
    for (auto& layer : layers_) {
        layer.setRandomWeights();
        for (auto& neuron : layer.neurons_) {
            neuron.deltas_ = neuron.output_ = 0;
        }
    }
}

void Matrix_perceptron::setDataInput(const std::vector<float>& normalize_input) noexcept {
    normalize_input_ = normalize_input;
}

void Matrix_perceptron::sumFunc() noexcept {
    for (int i = 0; i < layers_.size(); ++i) {
        Matrix_Layer& layer = layers_[i];

        const std::vector<float>& input_ = 
        (i == 0) 
        ? normalize_input_ 
        : layers_[i - 1].output_vector_; 

        layer.output_vector_.resize(layer.neurons_.size());

        for (int j = 0; j < layer.neurons_.size(); ++j)
        {
            Matrix_Neuron& neuron = layer.neurons_[j];

            float sum = neuron.bias_;

            for (int k = 0; k < neuron.weights_.size(); ++k) {
                sum += input_[k] * neuron.weights_[k];
            }

            neuron.output_ = neuron.sigmoidalFunc(sum);

            layer.output_vector_[j] = neuron.output_;
        }
    }
}

void Matrix_perceptron::updateWeights() noexcept {
    for (int l = 0; l < layers_.size(); ++l) {
        Matrix_Layer& layer = layers_[l];
        const std::vector<float>& input_ = (l == 0)
            ? normalize_input_
            : layers_[l - 1].output_vector_;

        for (auto& neuron : layer.neurons_) {
            neuron.bias_ -= LEARNING_RATE * neuron.deltas_;
            for (int w = 0; w < neuron.weights_.size(); ++w)
                neuron.weights_[w] -= LEARNING_RATE * neuron.deltas_ * input_[w];
        }
    }
}

float Matrix_perceptron::backPropagation(float expected_[OUTPUT_SIZE]) noexcept {

    Matrix_Layer& layer = layers_.back();

    //обработка выходного слоя
    for (int i = 0; i < layer.neurons_.size(); ++i) {
        Matrix_Neuron& neuron_ = layer.neurons_[i];
        float o = neuron_.output_;
        neuron_.deltas_ = (o - expected_[i]) * o * (1. - o);
        
    }

    //обработка слоев с конца
    for (int i = hidden_layers_count_; i > 0; i--) {
        Matrix_Layer& layer_current = layers_[i];
        Matrix_Layer& layer_next = layers_[i + 1];
        
        for (int g = 0; g < layer_current.neurons_.size(); ++g) {
            Matrix_Neuron& neuron = layer_current.neurons_[g];
            float error = 0.0;

            for (int n = 0; n < layer_next.neurons_.size(); ++n) {
                error += layer_next.neurons_[n].deltas_ * layer_next.neurons_[n].weights_[g];
            }
            neuron.deltas_ = error * neuron.output_ * (1.0 - neuron.output_);
        }
    }

    //обновление весов
    updateWeights();

    //считаем ошибку для конкретного примера (вектора значений), чтобы высчитать ошибку (или потерю) для эпохи
    float total_error_{};
    for (int i = 0; i < layer.neurons_.size(); ++i) {
        Matrix_Neuron& neuron_ = layer.neurons_[i];
        float diff_ = neuron_.output_ - expected_[i];
        total_error_+= diff_ * diff_;
    }

    return total_error_;
}

int Matrix_perceptron::Verify(const std::vector<float>& image) noexcept  {
    setDataInput(image);
    sumFunc();

    Matrix_Layer& last_layer_ = layers_[layers_.size() - 1];
    int best_index_{};
    float max_output_{last_layer_.neurons_[0].output_};

    for (int i = 0; i < last_layer_.neurons_.size(); ++i) {
        Matrix_Neuron& neuron_ = last_layer_.neurons_[i];
        if (neuron_.output_ > max_output_) {
            best_index_ = i;
            max_output_ = neuron_.output_;
        }
    }
    return best_index_;
}

float Matrix_perceptron::Train(const std::vector<EmnistData>& data) noexcept {

    float epoch_loss{};

    for (auto& input : data) {
        float y_training[OUTPUT_SIZE]{};
        
        y_training[input.index()] = 1;

        setDataInput(input.data());
        sumFunc();
        epoch_loss += backPropagation(y_training);
    }

    return epoch_loss / data.size();
}

void Matrix_perceptron::LoadWeights(const std::vector<float>& data) noexcept {
    int i{};

    for (auto& layer : layers_) {
        for (auto& neuron : layer.neurons_) {
            neuron.bias_ = data[i++];

            for (auto& weight : neuron.weights_) {
                weight = data[i++];
            }
        }
    }
}

std::vector<float> Matrix_perceptron::GetWeights() noexcept {
    std::vector<float> weights{};

    for (auto& layer : layers_) {
        for (auto& neuron : layer.neurons_) {
            weights.push_back(neuron.bias_);

            weights.insert(weights.end(), neuron.weights_.begin(), neuron.weights_.end());
        }
    }

    return weights;
}

}
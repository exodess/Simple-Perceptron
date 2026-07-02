#include "../../include/perceptron/matrix_perceptron.h"

namespace perc {

Matrix_perceptron::Matrix_perceptron(int hidden_layer_sizes) noexcept: Perceptron(MATRIX_VIEW, hidden_layer_sizes),
    hidden_layers_count_{hidden_layer_sizes}, layers_count{hidden_layer_sizes + 2}
{
    layers_.resize(layers_count);
    int temp_neuron_count_{};

    if (hidden_layer_sizes < MAX_HIDDEN) {
        temp_neuron_count_ = 64;
    } else {
        temp_neuron_count_ = 32;
    }

    layers_[layers_count - 1] = Matrix_Layer{temp_neuron_count_, COUNT_LETTERS};

    for (int i = layers_count - 2; i > 0; --i) {
        layers_[i] = Matrix_Layer{temp_neuron_count_ * 2, temp_neuron_count_};
        temp_neuron_count_ *= 2;
    }

    layers_[0] = Matrix_Layer{INPUT_SIZE, temp_neuron_count_};
}

Matrix_perceptron::Matrix_perceptron(int hidden_layer_sizes, vector<float> weights) noexcept: Perceptron(MATRIX_VIEW, hidden_layer_sizes),
    hidden_layers_count_{hidden_layer_sizes}, layers_count{hidden_layer_sizes + 2}
{
    layers_.resize(layers_count);
    int temp_neuron_count_{};

    if (hidden_layer_sizes < MAX_HIDDEN) {
        temp_neuron_count_ = 64;
    } else {
        temp_neuron_count_ = 32;
    }

    layers_[layers_count - 1] = Matrix_Layer{temp_neuron_count_, COUNT_LETTERS};

    for (int i = layers_count - 2; i > 0; --i) {
        layers_[i] = Matrix_Layer{temp_neuron_count_ * 2, temp_neuron_count_};
        temp_neuron_count_ *= 2;
    }

    layers_[0] = Matrix_Layer{INPUT_SIZE, temp_neuron_count_};
}

void Matrix_perceptron::Reset() noexcept {
    for (auto& layer : layers_) {
        layer.setRandomWeights();
        for (auto& neuron : layer.neurons_) {
            neuron.deltas_ = neuron.outputs_ = 0;
        }
    }
}

void Matrix_perceptron::setDataInput(const vector<float>& normalize_input) noexcept {
    normalize_input_ = normalize_input;
}

void Matrix_perceptron::sumFunc() noexcept {
    for (int i = 0; i < layers_count; ++i) {
        Matrix_Layer& layer = layers_[i];

        const vector<float>& input_ = 
        (i == 0) 
        ? normalize_input_ 
        : layers_[i - 1].output_vector_; 

        layer.output_vector_.resize(layer.neurons_.size());

        for (int i = 0; i < layer.neurons_.size(); ++i)
        {
            Matrix_Neuron& neuron = layer.neurons_[i];

            float sum = neuron.bias_;

            for (int k = 0; k < neuron.weights_.size(); ++k) {
                sum += input_[k] * neuron.weights_[k];
            }

            neuron.outputs_ = neuron.sigmoidalFunc(sum);

            layer.output_vector_[i] = neuron.outputs_;
        }
    }
}

void Matrix_perceptron::updateWeights() noexcept {
    for (int l = 0; l < layers_count; ++l) {
        Matrix_Layer& layer = layers_[l];
        const vector<float>& input_ = (l == 0)
            ? normalize_input_
            : layers_[l - 1].output_vector_;

        for (auto& neuron : layer.neurons_) {
            neuron.bias_ -= learning_rate_ * neuron.deltas_;
            for (int w = 0; w < neuron.weights_.size(); ++w)
                neuron.weights_[w] -= learning_rate_ * neuron.deltas_ * input_[w];
        }
    }
}

float Matrix_perceptron::backPropagation(float expected_[OUTPUT_SIZE]) noexcept {

    Matrix_Layer& layer = layers_.back();

    //обработка выходного слоя
    for (int i = 0; i < layer.neurons_.size(); ++i) {
        Matrix_Neuron& neuron_ = layer.neurons_[i];
        float o = neuron_.outputs_;
        neuron_.deltas_ = (o - expected_[i]) * o * (1. - o);
        
    }

    //обработка слоев с конца
    for (int i = layers_count - 2; i > -1; i--) {
        Matrix_Layer& layer_current = layers_[i];
        Matrix_Layer& layer_next = layers_[i + 1];
        
        for (int g = 0; g < layer_current.neurons_.size(); ++g) {
            Matrix_Neuron& neuron = layer_current.neurons_[g];
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
        Matrix_Neuron& neuron_ = layer.neurons_[i];
        float diff_ = neuron_.outputs_ - expected_[i];
        total_error_+= diff_ * diff_;
    }

    return total_error_;
}

int Matrix_perceptron::Verify(const vector<float>& image) noexcept  {
    setDataInput(image);
    sumFunc();

    Matrix_Layer& last_layer_ = layers_[layers_count - 1];
    int best_index_{};
    float max_output_{last_layer_.neurons_[0].outputs_};

    for (int i = 0; i < last_layer_.neurons_.size(); ++i) {
        Matrix_Neuron& neuron_ = last_layer_.neurons_[i];
        if (neuron_.outputs_ > max_output_) {
            best_index_ = i;
            max_output_ = neuron_.outputs_;
        }
    }
    return best_index_;
}

float Matrix_perceptron::Train(const std::vector<EmnistData>& data) noexcept {

    float epoch_loss{};

    for (auto& input : data) {
        float y_training[OUTPUT_SIZE]{};
        
        y_training[input.index() - 1] = 1;

        setDataInput(input.data());
        sumFunc();
        epoch_loss += backPropagation(y_training);
    }

    return epoch_loss;
}

void Matrix_perceptron::LoadWeights(const vector<float>& data) noexcept {
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

vector<float> Matrix_perceptron::GetWeights() noexcept {
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
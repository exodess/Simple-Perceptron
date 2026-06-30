#include "../../include/perceptron/perceptron.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

Neuron::Neuron() noexcept: deltas_{}, outputs_{} {}

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

    layers_[layers_count - 1] = Layer{temp_neuron_count_, COUNT_LETTERS};

    for (int i = layers_count - 2; i > 0; --i) {
        layers_[i] = Layer{temp_neuron_count_ * 2, temp_neuron_count_};
        temp_neuron_count_ *= 2;
    }

    layers_[0] = Layer{INPUT_SIZE, temp_neuron_count_};

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

    for (auto& neuron : neurons_) {
        neuron.weights_.resize(input_neurons_count_);
    }

    setRandomWeights();
}

void Matrix_perceptron::sumFunc() noexcept {
    for (int i = 0; i < layers_count; ++i) {
        Layer& layer = layers_[i];

        const vector<float>& input_ = 
        (i == 0) 
        ? normalize_input_ 
        : layers_[i - 1].output_vector_; 

        layer.output_vector_.resize(layer.neurons_.size());

        for (int i = 0; i < layer.neurons_.size(); ++i)
        {
            Neuron& neuron = layer.neurons_[i];

            float sum = neuron.bias_;

            for (int k = 0; k < neuron.weights_.size(); ++k)
                sum += input_[k] * neuron.weights_[k];

            neuron.outputs_ = neuron.sigmoidalFunc(sum);

            layer.output_vector_[i] = neuron.outputs_;
        }
    }
}

float Neuron::sigmoidalFunc(float x) noexcept {
    return  1. / (1. + exp(-x));
}

void Layer::setRandomWeights() noexcept {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.5, 0.5);

    for (auto& neuron : neurons_) {
        neuron.bias_ = dist(gen);
        for (auto& weight : neuron.weights_) {
            weight = dist(gen);
        }
    }
}

void Matrix_perceptron::updateWeights() noexcept {
    for (int l = 0; l < layers_count; ++l) {
        Layer& layer = layers_[l];
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

void Matrix_perceptron::training(int epoch, std::vector<EmnistData> EmnistData_) noexcept {

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(EmnistData_.begin(), EmnistData_.end(), g);

    training_set = vector<EmnistData>{EmnistData_.begin(), EmnistData_.begin() + 11839};
    test_set = vector<EmnistData>{EmnistData_.begin() + 11840, EmnistData_.end()};

    for (int j = 0; j < epoch; ++j) {

        float epoch_loss{};
        int correct = 0;

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(training_set.begin(), training_set.end(), g);

        for (auto& input : training_set) {

            float y_training[OUTPUT_SIZE]{};
            y_training[static_cast<int>(input.letter()) - 1] = 1;

            setDataInput(input.data());
            sumFunc();
            epoch_loss += backPropagation(y_training);
        }

        for (auto& input : test_set) {

            std::random_device rd;
            std::mt19937 t(rd());
            std::shuffle(training_set.begin(), training_set.end(), t);

            if (predict(input.data()) == static_cast<int>(input.letter()) - 1)
                correct++;
        }

        std::cout << "Epoch " << j + 1 
                << " loss: " << epoch_loss / EmnistData_.size() <<
                " average accuracy: " << (correct * 100. / 2960 ) << std::endl;
    }
}

Metrics::Metrics() noexcept: 
    accuracy{}, TP{}, FP{}, FN{} 
    {}

void Matrix_perceptron::experiment(float percentage) noexcept {

    Metrics metrix_;
    int test_size_ = static_cast<int>(percentage * test_set.size());

    // std::random_device rd;
    // std::mt19937 t(rd());
    // std::shuffle(training_set.begin(), training_set.end(), t);

    auto start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    for (int i = 0; i < test_size_; ++i) {
        EmnistData& input = test_set[i];
        int letter_ = static_cast<int>(input.letter()) - 1;
        int predict_ = predict(input.data());

        if (predict(input.data()) == letter_) {
            metrix_.accuracy++;
            metrix_.TP[predict_]++;
        } else {
            metrix_.FP[predict_]++;
            metrix_.FN[letter_]++;
        }
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "average accuracy: " << metrix_.accuracy * 100. / test_size_ << "\n";

    for (int i = 0; i < COUNT_LETTERS; ++i) {
        int precision_ = metrix_.TP[i] / (metrix_.TP[i] + metrix_.FP[i]);
        int recall_ = metrix_.TP[i] / (metrix_.TP[i] + metrix_.FP[i]);
        std::cout << "Metrics for " << char(65 + i)
        << " Precision: " << precision_
        << " Recall: " << recall_
        << " F-measure: " << 2. * (precision_ * recall_) / (precision_ + recall_);
    }

    std::cout << "\n" << "Total time spent " << duration.count() << " sec" << std::endl;

}

void Matrix_perceptron::readWeights(const vector<float>& data) {
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

vector<float> Matrix_perceptron::saveWeights() {
    vector<float> weights{};

    for (auto& layer : layers_) {
        for (auto& neuron : layer.neurons_) {
            for (auto& weight : neuron.weights_) {
                weights.push_back(weight);
            }
        }
    }
}
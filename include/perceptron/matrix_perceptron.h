#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "perceptron.h"

class Neuron {
public:
    explicit Neuron() noexcept;

    // Функция активации
    void sigmoidalFunc(double& x) noexcept;

    vector<double> weights_; // веса входящих связей
    double deltas_; // градиенты весов
    double outputs_;   // выходы после sigmoid

};

class Layer {
public:
    vector<Neuron> neurons_;
    vector<double> output_vector_;

    int input_neurons_count_;
    int output_neurons_count_;

    Layer() noexcept;
    Layer(int input_neurons_count_, int output_neurons_count_) noexcept;
    void setRandomWeights() noexcept;

};

class Matrix_perceptron : public Perceptron {
    double y_[OUTPUT_SIZE]{};
    int number_;


    int hidden_layers_count_;
    vector<Layer> layers_;
    vector<double> normalize_input_;

public:
    int layers_count;
    double learning_rate_{0.1};

    explicit Matrix_perceptron(int number, int hidden_layer_sizes) noexcept;

    void sumFunc() noexcept;
    
    // Обратное распространение ошибки
    void backPropagation() noexcept;

    // Возвращает индекс буквы
    int predict() noexcept;

    void training() noexcept;
};

#endif
#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "perceptron.h"

class Neuron {
public:
    explicit Neuron(vector<double>& weights_) noexcept;

    // Функция активации
    void sigmoidalFunc(double& x);

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
    char letter_;


    int hidden_layers_count_;
    vector<Layer> layers_;
    vector<double> normalize_input_;

public:
    int layers_count;
    double learning_rate_{0.1};

    explicit Matrix_perceptron(char letter, int hidden_layer_sizes); // Создание нейрона с привязанной к нему буквой
    explicit Matrix_perceptron(int number, int hidden_layer_sizes) noexcept; // Создание нейрона с порядковым номером буквы в алфавите

    void sumFunc();

    // Обратное распространение ошибки
    void backPropagation();

    // Возвращает индекс буквы
    int predict();

    void training() noexcept;
};

#endif
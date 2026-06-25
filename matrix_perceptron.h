#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "perceptron.h"


// Слой представляет из себя N паралелльных несвязанных между собой нейронов

class Neuron {
public:
    explicit Neuron(vector<double>& weights_) noexcept;

    // Функция активации
    void sigmoidalFunc(double& x);

    char letter_; // буква, с которой ассоциируется нейрон

    vector<double> weights_; // веса входящих связей
    double deltas_; // градиенты весов
    double outputs_;   // выходы после sigmoid

};

class Layer {
public:
    std::vector<Neuron> neurons_;

    int input_neurons_count_;
    int output_neurons_count_;

    Layer() noexcept;
    Layer(int input_neurons_count_, int output_neurons_count_) noexcept;

};

class Matrix_perceptron : public Perceptron {
    double y_;  // ожидаемый ответ (0 или 1 — эта буква или нет)

    vector<Layer> layers_;
    vector<double> normalize_input_;

public:
    int layers_count;
    double learning_rate_{0.1};

    explicit Matrix_perceptron(char letter, vector<int>& layer_sizes) noexcept; // Создание нейрона с привязанной к нему буквой
    explicit Matrix_perceptron(int number, vector<int>& layer_sizes) noexcept; // Создание нейрона с порядковым номером буквы в алфавите

    void sumFunc();

    // Обратное распространение ошибки
    void backPropagation();

    // Возвращает индекс буквы
    int predict();

    void training() noexcept;
};

#endif
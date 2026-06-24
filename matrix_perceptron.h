#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#define MAX_WIDTH_RES 512
#define MAX_HEIGHT_RES 512
#define INPUT_SIZE  784
#define OUTPUT_SIZE 26
#define MIN_HIDDEN  2
#define MAX_HIDDEN  5
#define MATRIX_SIZE 28
#define MAX_NEURONS 64

#include <vector>

using std::vector;


// Слой представляет из себя N паралелльных несвязанных между собой нейронов
class Layer {
public:
    double weights_[MAX_NEURONS][INPUT_SIZE]; // веса входящих связей
    double grad_output_[INPUT_SIZE]; // градиенты весов
    double outputs_[INPUT_SIZE];   // выходы после sigmoid

    int input_size_;
    int output_size_;

    Layer() noexcept;
    Layer(double weights_[MAX_WIDTH_RES][MAX_HEIGHT_RES], 
        double normalize_input_[MAX_WIDTH_RES][MAX_HEIGHT_RES]) noexcept;

};

class Matrix_perceptron {
    double y_;  // ожидаемый ответ (0 или 1 — эта буква или нет)

    vector<Layer> layers_;
    int input_[MAX_WIDTH_RES][MAX_HEIGHT_RES];
    double normalize_input_[INPUT_SIZE];

public:
    char letter_; // буква, с которой ассоциируется нейрон
    int layers_count;

    double min_; 
    double max_;
    double learning_rate_{0.1};

    explicit Matrix_perceptron(char letter, vector<int> layer_sizes) noexcept; // Создание нейрона с привязанной к нему буквой
    explicit Matrix_perceptron(int number) noexcept; // Создание нейрона с порядковым номером буквы в алфавите

    // Задание нового количества скрытых слоев
    void setCountLayer(int value) noexcept;

    // Устанавливаем рандомные веса
    void createWeights();

    // Минимум и максимум входных значений
    void setMinMax();

    // Функция нормировки
    void rationing();

    // Функция активации
    void sumFunc(int count);
    
    // Функция активации
    void sigmoidalFunc(double& x);
    
    // Расчет функции ошибки
    double errorFunc(int count);

    // Обратное распространение ошибки
    void backPropagation(int count);

    // Возвращает индекс буквы
    int predict();

    void training() noexcept;
};

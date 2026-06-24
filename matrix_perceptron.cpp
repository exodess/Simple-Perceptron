#include "matrix_perceptron.h"

#include <cmath>
#include <algorithm>

void Matrix_perceptron::setCountLayer(int value) noexcept {
    inner_layers_count = value;
}

void Matrix_perceptron::setMinMax() {
    min_ = input_[0][0];
    max_ = input_[0][0];

    for (int i = 0; i < MAX_HEIGHT_RES; ++i) {
        for (int j = 0; j < MAX_WIDTH_RES; ++j) {
            if (min_ > input_[i][j]) min_ = input_[i][j];
            if (max_ < input_[i][j]) max_ = input_[i][j];
        }
    }
}

void Matrix_perceptron::rationing() {
    double diff = max_ - min_;
    int normalize_input_i{};
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            normalize_input_[normalize_input_i++] = (static_cast<double>(input_[i][j]) - min_) / diff;
        }
    }
}

void Matrix_perceptron::sumFunc(int count) {
    const double* current_input_ = (count == 0) 
        ? normalize_input_ 
        : layers_[count - 1].outputs_;

    for (int i = 0; i < layers_[count].output_size_; ++i) {
        layers_[count].outputs_[i] = 0;
        for (int j = 0; j < layers_[count].input_size_; ++j) {
            layers_[count].outputs_[i] += current_input_[j] * layers_[count].weights_[i][j];
        }
        sigmoidalFunc(layers_[count].outputs_[i]);
    }
}

void Matrix_perceptron::sigmoidalFunc(double& x) {
    x = 1. / (1. + exp(-x));
}

double Matrix_perceptron::errorFunc(int count) {
    double square_diff_{}, L_{};

    for (int i = 0; i < layers_[count].output_size_; ++i) {
        square_diff_ += pow(layers_[count].outputs_[i] - y_, 2);
    }

    L_ = square_diff_ / layers_[count].output_size_;

    return L_;
}

void Matrix_perceptron::backPropagation(int count) {

    const double* current_input_ = (count == 0) 
        ? normalize_input_ 
        : layers_[count - 1].outputs_;

    double L_ = errorFunc(count);

    for (int i = 0; i < layers_[count].output_size_; ++i) {
        for (int j = 0; j < layers_[count].input_size_; ++j) {
            double diff_weight_ = 2 * (layers_[count].outputs_[i] - y_) * current_input_[j];
            layers_[count].weights_[i][j] -= learning_rate_ * diff_weight_;
        }
    }
}

void Matrix_perceptron::training() noexcept {
    setMinMax();
    rationing();
    for (int i = 0; i < layers_count; ++i) {
        sumFunc(i);
    }
    for (int i = layers_count - 1; i > -1; --i) {
        backPropagation(i);
    }
}
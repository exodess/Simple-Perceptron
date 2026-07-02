#ifndef MATRIX_PERCEPTRON_H
#define MATRIX_PERCEPTRON_H

#include "perceptron.h"

namespace perc {

/**
 * @class Matrix_Neuron
 * @brief Класс нейрона, содержащий основные параметры для работы с выходными значениями.
 * Составляет слои перцептрона
 */

class Matrix_Neuron {
public:

    Matrix_Neuron() noexcept;

    ~Matrix_Neuron() = default;

    /**
     * @brief Функция активации для выходного значения нейрона
     */
    float sigmoidalFunc(float x) noexcept;

    vector<float> weights_; ///< Веса входящих связей
    float deltas_; ///< Градиенты весов
    float outputs_; ///< Выходы после sigmoid
    float bias_; ///< Смещение

};

/**
 * @class Matrix_Layer
 * @brief Класс слоя, содержащий нейроны и информацию о них.
 * На первой итерации устанавливает рандомные веса каждому из нейронов
 */
class Matrix_Layer {
public:

    Matrix_Layer() noexcept;

    Matrix_Layer(int input_neurons_count_, int output_neurons_count_) noexcept;

        ~Matrix_Layer() = default;

    /**
     * @brief Метод, устанавливающий рандомные веса один раз 
     * в самом начале обучения
     */
    void setRandomWeights() noexcept;

    vector<Matrix_Neuron> neurons_; ///< Вектор нейронов слоя
    vector<float> output_vector_; ///< Вектор выходных значений нейронов слоя

    int input_neurons_count_; ///< Количество входных нейронов
    int output_neurons_count_; ///< Количество выходных нейронов
};

/**
 * @class Matrix_perceptron
 * @brief Класс, описывающий основные методы
 * обучения и работы перцептрона в матричной реализации
 */
class Matrix_perceptron : public Perceptron {
public:

    explicit Matrix_perceptron(int hidden_layer_sizes) noexcept;

    explicit Matrix_perceptron(int hidden_layer_sizes, vector<float> weights) noexcept;

    ~Matrix_perceptron() = default;

    void Reset() noexcept override;

    int Verify(const vector<float>& image) noexcept override;

    float Train(const std::vector<EmnistData>& data) noexcept override;

    void LoadWeights(const vector<float>& data) noexcept override;

    vector<float> GetWeights() noexcept override;

    /**
     * @brief Метод, который устанавливает новые данные для перцептрона.
     * @param vector<float> normalize_input вектор входных значений
     */
    void setDataInput(const vector<float>& normalize_input) noexcept;

    /**
     * @brief Метод, который делает полный проход по слоям перцептрона,
     * суммирует выходы нейронов предыдущего слоя, умноженные на текущие веса.
     * На последнем R слое получаются 26 нейронов с выходным значением для каждого (outputs_).
     */
    void sumFunc() noexcept;

    /**
     * @brief Метод, который делает полный проход по слоям в обратном направлении
     * согласно алгоритму обратного распространения ошибки. Обновляем веса по 
     * результатам разницы между полученныи выходом нейрона (outputs_) и 
     * ожидаем результатом y_[i], где i - индекс нейрона, y_[i] - ожидаемый
     * для текущего нейрона выход.
     * Используется только для обучения
     */
    float backPropagation(float expected_[OUTPUT_SIZE]) noexcept;

    /**
     * @brief Метод, который задает рандомные значения весам
     */
    void updateWeights() noexcept;

private:

    int layers_count; ///< Количество всех слоев перцептрона
    int hidden_layers_count_; ///< Количество скрытых слоев
    int epochs_; ///< Количество эпох обучения

    vector<Matrix_Layer> layers_; ///< вектор всех слоев нейрона, где layers_[0] - входной слой, layers_[layers_count - 1] - выходной
    vector<float> normalize_input_; ///< вектор входных значений для одного изображения

    float learning_rate_{0.1}; ///< Шаг обучения

    PerceptronType Perceptron_type_{MATRIX_VIEW}; ///< Тип реализации
};

}

#endif

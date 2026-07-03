#ifndef MATRIX_PERCEPTRON_H
#define MATRIX_PERCEPTRON_H

#include "perceptron.h"

/**
 * @file matrix_perceptron.h
 * @brief В этом файле находятся класс перцептрона в матричной реализации и его составляющие (классы матричного слоя и матричного нейрона)
 * @author Tarasova Alina
 */

namespace perc {

/**
 * @class Matrix_Neuron
 * @brief Класс нейрона, содержащий основные параметры для работы с выходными значениями.
 * Составляет слои перцептрона
 */

class Matrix_Neuron : public Neuron {
public:
    Matrix_Neuron() noexcept;

    explicit Matrix_Neuron(int inputs_count) noexcept;

    std::vector<float> weights_;
};

/**
 * @class Matrix_Layer
 * @brief Класс слоя, содержащий нейроны и информацию о них.
 * На первой итерации устанавливает рандомные веса каждому из нейронов
 */
class Matrix_Layer : public Layer {
public:

    Matrix_Layer() noexcept;

    Matrix_Layer(int input_neurons_count_, int output_neurons_count_) noexcept;

    ~Matrix_Layer() = default;

    /**
     * @brief Метод, устанавливающий рандомные веса один раз 
     * в самом начале обучения
     */
    void setRandomWeights() noexcept;

    std::vector<Matrix_Neuron> neurons_; ///< Вектор нейронов слоя
    std::vector<float> output_vector_; ///< Вектор выходных значений нейронов слоя
};

/**
 * @class Matrix_perceptron
 * @brief Класс, описывающий основные методы
 * обучения и работы перцептрона в матричной реализации
 */
class Matrix_perceptron final : public Perceptron {
public:

    explicit Matrix_perceptron(int hidden_layer_sizes) noexcept;

    ~Matrix_perceptron() = default;

    void Reset() noexcept override;

    int Verify(const std::vector<float>& image) noexcept override;

    float Train(const std::vector<EmnistData>& data) noexcept override;

    void LoadWeights(const std::vector<float>& data) noexcept override;

    std::vector<float> GetWeights() noexcept override;

private:

    /**
     * @brief Метод, который устанавливает новые данные для перцептрона.
     * @param std::vector<float> normalize_input вектор входных значений
     */
    void setDataInput(const std::vector<float>& normalize_input) noexcept;

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

    PerceptronType Perceptron_type_{MATRIX_VIEW}; ///< Тип реализации

    int layers_count; ///< Количество всех слоев перцептрона
    std::vector<Matrix_Layer> layers_; ///< вектор всех слоев нейрона, где layers_[0] - входной слой, layers_[layers_count - 1] - выходной

    std::vector<float> normalize_input_; ///< вектор входных значений для одного изображения
    float learning_rate_{0.1}; ///< Шаг обучения
};

}

#endif

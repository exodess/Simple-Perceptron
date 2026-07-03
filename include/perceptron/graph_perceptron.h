#ifndef GRAPH_PERCEPTRON_H
#define GRAPH_PERCEPTRON_H

#include "perceptron.h"

/**
 * @file matrix_perceptron.h
 * @brief В этом файле находятся класс перцептрона в графовой реализации и его составляющие (классы графового слоя и графового нейрона)
 * @author Tarasova Alina
 */

namespace perc {

/**
 * @class Graph_Neuron
 * @brief Класс нейрона, содержащий основные параметры для работы с выходными значениями.
 * Составляет слои перцептрона
 */

class Graph_Neuron;

/**
 * @class Edge
 * @brief Класс ребра между двумя нейронами
 */

struct Edge {

    Edge() noexcept;

    explicit Edge(Graph_Neuron* from, Graph_Neuron* to, float weight) noexcept;

    Graph_Neuron* from_; ///< Указатель на начало ребра (нейрон)
    Graph_Neuron* to_; ///< Указатель на конец ребра (нейрон)
    float weight_; ///< Передаваемый вес между нейронами
};

class Graph_Neuron : public Neuron {
public:
    Graph_Neuron() noexcept;

    std::vector<Edge*> inputs_; ///< Вектор граней (связи от текущего нейрона ко всем в предыдущем слое)
    std::vector<Edge*> outputs_; ///< Вектор граней (связи от текущего нейрона ко всем в следующем слое)
};

/**
 * @class Graph_Layer
 * @brief Класс слоя, содержащий нейроны и информацию о них.
 */
class Graph_Layer : public Layer{
public:

    Graph_Layer() noexcept;

    explicit Graph_Layer(int input_neurons_count_, int output_neurons_count_) noexcept;

    std::vector<Graph_Neuron> neurons_;  ///< Вектор нейронов слоя
    std::vector<float> output_vector_; ///< Вектор выходов нейронов слоя
};

/**
 * @class Graph_perceptron
 * @brief Класс, описывающий основные методы
 * обучения и работы перцептрона в графовой реализации
 */

class Graph_perceptron final : public Perceptron { 
public:
    explicit Graph_perceptron(int hidden_layer_sizes) noexcept;

    ~Graph_perceptron() = default;

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
     * @brief Метод, который обновляет веса в ходе обучения
     */
    void updateWeights() noexcept;

    /**
     * @brief Метод, который задает рандомные значения весам
     */
    float setRandomWeight() noexcept;

    PerceptronType Perceptron_type_{GRAPH_VIEW}; ///< Тип реализации

    int layers_count; ///< Количество всех слоев перцептрона
    std::vector<Graph_Layer> layers_; ///< вектор всех слоев нейрона, где layers_[0] - входной слой, layers_[layers_count - 1] - выходной
    std::vector<Edge> all_edges_; ///< вектор граней между нейронами

    std::vector<float> normalize_input_; ///< вектор входных значений для одного изображения
    float learning_rate_{0.1}; ///< Шаг обучения

};

}

#endif

#ifndef GRAPH_PERCEPTRON_H
#define GRAPH_PERCEPTRON_H

#include "perceptron.h"

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

    Edge(Graph_Neuron* from, Graph_Neuron* to, float weight) noexcept;

    Graph_Neuron* from_; ///< Указатель на начало ребра (нейрон)
    Graph_Neuron* to_; ///< Указатель на конец ребра (нейрон)
    float weight_; ///< Передаваемый вес между нейронами
};

class Graph_Neuron {
public:

    Graph_Neuron() noexcept;

    Graph_Neuron(vector<Edge> edges_) noexcept;

    ~Graph_Neuron() = default;

    /**
     * @brief Функция активации для выходного значения нейрона
     */
    float sigmoidalFunc(float x) noexcept;

    /**
     * @brief Метод, устанавливающие рандомные значения весов нейронов
     */
    float setRandomWeight() noexcept;

    vector<Edge*> inputs_; ///< Вектор граней (связи от текущего нейрона ко всем в предыдущем слое)
    vector<Edge*> outputs_; ///< Вектор граней (связи от текущего нейрона ко всем в следующем слое)

    float deltas_; ///< Градиенты весов
    float output_; ///< Выходы после sigmoid
    float bias_; ///< Смещение
};

/**
 * @class Graph_Layer
 * @brief Класс слоя, содержащий нейроны и информацию о них.
 */
class Graph_Layer {
public:

    Graph_Layer() noexcept;

    Graph_Layer(int input_neurons_count_, int output_neurons_count_) noexcept;

    ~Graph_Layer() = default;

    vector<Graph_Neuron> neurons_; ///< Вектор нейронов слоя

    int input_neurons_count_; ///< Количество входных нейронов
    int output_neurons_count_; ///< Количество выходных нейронов

};

/**
 * @class Graph_perceptron
 * @brief Класс, описывающий основные методы
 * обучения и работы перцептрона в графовой реализации
 */

class Graph_perceptron : public Perceptron { 
public:
    explicit Graph_perceptron(int hidden_layer_sizes) noexcept;

    explicit Graph_perceptron(int hidden_layer_sizes, vector<float>& weights) noexcept;

    ~Graph_perceptron() = default;

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

    vector<Graph_Layer> layers_; ///< вектор всех слоев нейрона, где layers_[0] - входной слой, layers_[layers_count - 1] - выходной
    vector<Edge> all_edges_; ///< вектор граней между нейронами
    vector<float> normalize_input_; ///< вектор входных значений для одного изображения

    float learning_rate_{0.1}; ///< Шаг обучения

    PerceptronType Perceptron_type_{GRAPH_VIEW}; ///< Тип реализации
};

}

#endif

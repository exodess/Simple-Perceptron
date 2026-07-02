#ifndef CLASS_PERCEPTRON_H
#define CLASS_PERCEPTRON_H

/**
 * @file perceptron.h
 * @brief В этом файле находятся базовый класс перцептрона, классы его реализации
 * в матричном и графовом видах и их составляющие (классы слоя и нейрона).
 * Также здесь описаны основные алгоритмы/методы по обучению перцептрона.
 * @author Tarasova Alina
 */

#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <iostream>

#include "../data/data.h"
#include "../tools/tools.h"

using std::vector;
using std::unique_ptr;

/**
 * @brief 
 * Способы представления перцептрона: матричный и графовый
 */
enum PerceptronType {
    MATRIX_VIEW,
    GRAPH_VIEW
};

/**
 * @class Perceptron
 * @brief Абстрактный класс, описывающий основные методы
 * обучения и работы перцептрона
 */
class Perceptron {
public:
    /**
     * @brief Инициализация класса перцептрона
     * @param index Индекс буквы для обучения и предсказания
     * @param hidden_layers Количество скрытых слоев (от 2 до 5)
     */
    Perceptron(int index, int hidden_layers) noexcept;
    virtual ~Perceptron() = default;

    /**
     * @brief Возвращает индекс буквы
     */
    int index() noexcept { return index_; }

    /**
     * @brief Возвращает количество скрытых слоев
     */
    int hiddenLayers() noexcept { return hidden_layers_count_; }

    /**
     * @brief Приводит перцептрон к стандартному состоянию
     * (как будто объект только создан)
     */
    virtual void Reset() noexcept = 0;

    /**
     * @brief Метод, возвращающий индекс нейрона с максимальный выходом
     * @return Индекс предсказанного значения
     */
    virtual int Verify(const vector<float>& image) noexcept = 0;

    /**
     * @brief Метод, обучающий перцептрон. Собирает в себе
     * суммирование произведений входных значений на веса и
     * метод обратного распространения. Работает для датасета,
     * обрабатывается целиком вектор изображения по очереди.
     * @param data Массив выборок, где каждый элемент содержит данные о конкретной картинке
     * @return Контрольное значение ошибки после обучения
     */
    virtual float Train(std::vector<perc::EmnistData> data) noexcept = 0;

    /**
     * @brief Осуществляет доступ к весам перцептрона для того,
     * чтобы загрузить их в файл с помощью DataReader или сохранить в перцептроне
     * @return
     */
    virtual vector<float>& Weights() noexcept = 0;

protected:
    int index_; ///< Индекс буквы
    int hidden_layers_count_; ///< Количество скрытых слоев перцептрона

};

/**
 * @class Neuron
 * @brief Класс нейрона, содержащий основные параметры для работы с выходными значениями.
 * Составляет слои перцептрона
 */
class Neuron {
public:

    Neuron() noexcept;

    /**
     * @brief Функция активации для выходного значения нейрона
     */
    float sigmoidalFunc(float x) noexcept;

    vector<float> weights_; ///< Веса входящих связей
    float deltas_; ///< Градиенты весов
    float outputs_; ///< Выходы после sigmoid
    float bias_; ///< Смещение

    ~Neuron() = default;

};

/**
 * @class Layer
 * @brief Класс слоя, содержащий нейроны и информацию о них.
 * На первой итерации устанавливает рандомные веса каждому из нейронов
 */
class Layer {
public:

    Layer() noexcept;
    Layer(int input_neurons_count_, int output_neurons_count_) noexcept;

    /**
     * @brief Метод, устанавливающий рандомные веса один раз 
     * в самом начале обучения
     */
    void setRandomWeights() noexcept;

    vector<Neuron> neurons_; ///< Вектор нейронов слоя
    vector<float> output_vector_; ///< Вектор выходных значений нейронов слоя

    int input_neurons_count_; ///< Количество входных нейронов
    int output_neurons_count_; ///< Количество выходных нейронов

    ~Layer() = default;

};

/**
 * @class Matrix_perceptron
 * @brief Класс, описывающий основные методы
 * обучения и работы перцептрона в матричной реализации
 */
class Matrix_perceptron : public Perceptron {
public:

    explicit Matrix_perceptron(int number, int hidden_layer_sizes) noexcept;

    explicit Matrix_perceptron(int number, int hidden_layer_sizes, vector<float> weights) noexcept;

    ~Matrix_perceptron() = default;

    /**
     * @brief Метод, который устанавливает количество скрытых слоев
     * @param value количество скрытых эпох
     */
    void setHiddenLayer(int value) noexcept override;

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

    /**
     * @brief Метод, который делает предсказание по переданному изображению (данными)
     * @param const vector<float>& image вектор входных значений
     */
    int verify(const vector<float>& image) noexcept override;

    /**
     * @brief Метод, который обучает перцептрон на выборке
     * @param epoch количество эпох
     * @param std::vector<EmnistData>& EmnistData_ вектор объектов EmnistData,
     * которые содержат вектор входных значений(изображения) и индекс буквы
     */
    vector<float> training(std::vector<EmnistData> EmnistData_, int epoch) noexcept override;

     /**
     * @brief Метод, который тестирует перцептрон на тестовой выборке
     * @param percentage соотношение тестируемой выборки, задается дробным числом
     */
    void experiment(float percentage) noexcept override;

    void loadWeights(const vector<float>& data) noexcept override;

    vector<float> saveWeights() noexcept override;

    PerceptronType& type() noexcept override;

    int number() noexcept override;

    int hidden_layers() noexcept override;

private:

    float y_[OUTPUT_SIZE]{}; ///< вектор ожидаемых значений (все значения равны нулю кроме y_[i], где i - индекс буквы)
    int number_; ///< Индекс буквы

    int layers_count; ///< Количество всех слоев перцептрона
    int hidden_layers_count_; ///< Количество скрытых слоев
    int epochs_; ///< Количество эпох обучения

    vector<Layer> layers_; ///< вектор всех слоев нейрона, где layers_[0] - входной слой, layers_[layers_count - 1] - выходной
    vector<float> normalize_input_; ///< вектор входных значений для одного изображения

    vector<EmnistData> test_set; ///< тестовая выборка для обучения и эксперимента, формурется в training
    vector<EmnistData> training_set; ///< тренировочная выборка

    float learning_rate_{0.1}; ///< Шаг обучения

    PerceptronType Perceptron_type_{MATRIX_VIEW}; ///< Тип реализации
};

class Graph_perceptron : public Perceptron { };

struct Metrics {
    float accuracy;
    int TP[COUNT_LETTERS];
    int FP[COUNT_LETTERS];
    int FN[COUNT_LETTERS];

    Metrics() noexcept;
};

#endif
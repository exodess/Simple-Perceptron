#ifndef CLASS_PERCEPTRON_H
#define CLASS_PERCEPTRON_H

/**
 * @file perceptron.h
 * @brief В этом файле находятся базовый класс перцептрона и его составляющие (классы слоя и нейрона)
 * @author Tarasova Alina
 */

#include "tools/tools.h"
#include "data/data.h"
#include <memory>
#include <random>
#include <algorithm>
#include <vector>
#include <cmath>

namespace perc {

    /**
     * @enum PerceptronType
     * @brief Описывает способы конечной реалзизации перцептрона
     */
    enum PerceptronType {
        MATRIX_VIEW, ///< Матричный вид
        GRAPH_VIEW ///< С помощью графов
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
         * @param hidden_layers Количество скрытых слоев (от 2 до 5)
         */
        Perceptron(PerceptronType type, int hidden_layers) noexcept: type_(type), hidden_layers_count_(hidden_layers) {}

        virtual ~Perceptron() = default;

        /**
         * @brief Передает реализацию перцептрона
         */
        PerceptronType type() noexcept { return type_; }

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
        virtual int Verify(const std::vector<float>& image) noexcept = 0;

        /**
         * @brief Метод, обучающий перцептрон. Собирает в себе
         * суммирование произведений входных значений на веса и
         * метод обратного распространения. Работает для датасета,
         * обрабатывается целиком вектор изображения по очереди.
         * @param data Массив выборок, где каждый элемент содержит данные о конкретной картинке
         * @return Контрольное значение ошибки после обучения
         */
        virtual float Train(const std::vector<EmnistData>& data) noexcept = 0;

        /**
         * @brief Метод, загружающий веса в перцептрон.
         * Веса можно загрузить либо при создании перцептрона (перегрузка конструктора),
         * либо создать объект перцептрона и вызвать функцию на загрузку
         * @param data Массив весов для всех нейронов
         */
        virtual void LoadWeights(const std::vector<float>& data) noexcept = 0;

        /**
         * @brief Метод, возращающий веса перцептрона.
         * @return Массив весов для всех нейронов (конкретного количества скрытых слоев)
         */
        virtual std::vector<float> GetWeights() noexcept = 0;

    protected:
        PerceptronType type_; ///< Тип перцептрона
        int hidden_layers_count_; ///< Количество скрытых слоев перцептрона

    };

    /**
     * @class Neuron
     * @brief Базовый класс нейрона
     */
    class Neuron {
    public:
        Neuron() noexcept: deltas_{}, output_{}, bias_{} {}

        /**
        * @brief Функция активации для выходного значения нейрона
        */
        float sigmoidalFunc(float x) noexcept {
            return 1. / (1. + exp(-x));
        };

        float deltas_; ///< Градиенты весов
        float output_; ///< Выходы после sigmoid
        float bias_; ///< Смещение
    };

    /**
     * @class Layer
     * @brief Базовый класс слоя
     */
    class Layer {
    public:
        Layer() noexcept: 
            input_neurons_count_{}, output_neurons_count_{} {}

        Layer(int input_neurons_count_, int output_neurons_count_) noexcept: 
            input_neurons_count_{input_neurons_count_}, output_neurons_count_{output_neurons_count_} {}
    
        int input_neurons_count_; ///< Количество входных нейронов
        int output_neurons_count_; ///< Количество выходных нейронов
    };

}

#endif
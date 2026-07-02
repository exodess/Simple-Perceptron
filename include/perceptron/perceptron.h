#ifndef CLASS_PERCEPTRON_H
#define CLASS_PERCEPTRON_H

/**
 * @file perceptron.h
 * @brief В этом файле находятся базовый класс перцептрона и его составляющие (классы слоя и нейрона)
 * @author Tarasova Alina
 */

#include "tools/tools.h"
#include <vector>

using std::vector;

namespace perc {
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
        virtual float Train(const std::vector<EmnistData>& data) noexcept = 0;

        /**
         * @brief Осуществляет доступ к весам перцептрона для того,
         * чтобы загрузить их в файл с помощью DataReader или сохранить в перцептроне
         * @return Массив всех весов
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
}

#endif
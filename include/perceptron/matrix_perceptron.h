#ifndef CLASS_MATRIX_PERCEPTRON_H
#define CLASS_MATRIX_PERCEPTRON_H

/**
 * @file matrix_perceptron.h
 * @brief В этом файле находится описание матричной реализации перцептрона
 * @author Tarasova Alina
 */

#include <data/data.h>
#include <perceptron/perceptron.h>

namespace perc {

    /**
     * @class MatrixPerceptron
     * @brief Класс, описывающий основные методы
     * обучения и работы перцептрона в матричной реализации
     */
    class MatrixPerceptron final : public Perceptron {
    public:
        explicit MatrixPerceptron(int number, int hidden_layer_sizes) noexcept;
        ~MatrixPerceptron() = default;

        /**
         * @brief Приводит перцептрон к начальному состонию
         */
        void Reset() noexcept override;

        /**
         * @brief Метод, который делает предсказание по переданному изображению (данными)
         * @param image вектор входных значений
         */
        int Verify(const vector<float>& image) noexcept override;

        /**
         * @brief Метод, который обучает перцептрон на выборке
         * @param dataset вектор объектов EmnistData,
         * которые содержат вектор входных значений(изображения) и индекс буквы
         */
        float Train(const std::vector<EmnistData>& dataset) noexcept override;

        /**
         * @brief Осуществляет доступ к весам перцептрона для того,
         * чтобы загрузить их в файл с помощью DataReader или сохранить в перцептроне
         * @return Массив всех весов
         */
        vector<float>& Weights() noexcept override;

    private:
        /**
         * @brief Метод, который делает полный проход по слоям перцептрона,
         * суммирует выходы нейронов предыдущего слоя, умноженные на текущие веса.
         * На последнем R слое получаются 26 нейронов с выходным значением для каждого (outputs_).
         */
        void _sumFunc() noexcept;

        /**
         * @brief Метод, который делает полный проход по слоям в обратном направлении
         * согласно алгоритму обратного распространения ошибки. Обновляем веса по
         * результатам разницы между полученныи выходом нейрона (outputs_) и
         * ожидаем результатом y_[i], где i - индекс нейрона, y_[i] - ожидаемый
         * для текущего нейрона выход.
         * Используется только для обучения
         */
        float _backPropagation(const vector<float>& expected) noexcept;

        /**
         * @brief Метод, который задает рандомные значения весам
         */
        void _updateWeights() noexcept;

        float y_[OUTPUT_SIZE]{}; ///< вектор ожидаемых значений (все значения равны нулю кроме y_[i], где i - индекс буквы)
        int number_; ///< Индекс буквы

        int hidden_layers_count_; ///< Количество скрытых слоев

        vector<Layer> layers_; ///< вектор всех слоев нейрона, где layers_[0] - входной слой, layers_[layers_count - 1] - выходной
        vector<float> normalize_input_; ///< вектор входных значений для одного изображения

        vector<EmnistData> test_set; ///< тестовая выборка для обучения и эксперимента, формурется в training
        vector<EmnistData> training_set; ///< тренировочная выборка
    };
}

#endif
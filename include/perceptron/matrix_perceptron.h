#ifndef CLASS_MATRIX_PERCEPTRON_H
#define CLASS_MATRIX_PERCEPTRON_H

/**
 * @file matrix_perceptron.h
 * @brief В этом файле находится описание матричной реализации перцептрона
 * @author Tarasova Alina
 */

#include <perceptron/perceptron.h>

namespace perc {

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
}

#endif
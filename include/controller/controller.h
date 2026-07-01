#ifndef PERCEPTRON_CONTROLLER_H
#define PERCEPTRON_CONTROLLER_H

/**
 * @file controller.h
 * @brief В этом файле находится описание класса Controller и вспомогательные структуры,
 * используемые для более наглядного представления работы перцептрона
 * @author Georgiy Kovalev
 */

#include "tools/tools.h"
#include "perceptron/perceptron.h"
#include <memory>

namespace perc {
    /**
     * @struct SuccessRate
     * @brief Информация об успешности проведенного тестирования. Позволяет понять,
     * насколько хорошо реализован перцептрон
     */
    struct SuccessRate {
        SuccessRate() noexcept :
        average_accuracy_(-1.0f), precision_(-1.0f), recall_(-1.0f), f_measure_(-1.0f), time_spent_(0.0f) {}

        SuccessRate(float acc, float prec, float recall, float measure, float time) noexcept :
        average_accuracy_(acc), precision_(prec), recall_(recall), f_measure_(measure), time_spent_(time) {}

        float& accuracy() noexcept { return average_accuracy_; }
        float& precision() noexcept { return precision_; }
        float& recall() noexcept { return recall_; }
        float& f_measure() noexcept { return f_measure_; }
        float& time() noexcept { return time_spent_; }

    private:
        float average_accuracy_; ///< Общая доля правильных ответов среди всех предсказаний
        float precision_; ///< Степень близости друг к другу независимых результатов измерений
        float recall_; ///< Способност перцептрона находить все объекты нужного класса в датасете
        float f_measure_; ///< Универсальный показатель качества работы перцептрона (среднее между precision и recall)
        float time_spent_; ///< Общее затраченное время
    };

    /**
    * @class Controller
    * @brief Данный класс осуществляет управление всей логикой программы.
    * Реализует паттерн Facade - хранит в себе все остальные классы Backend'a.\n
    * Принцип взаимодействия:
    * - Пользователь совершает действие с GUI
    * - GUI отправляет сигнал контрллеру вместе с данными, которые передал пользователь
    * - Контроллер приводит в действие необходимый класс внутри себя
     */
    class Controller {
    public:
        /**
         * @brief Создание класса контроллера.
         * Внутри себя создает объекты классов внутренней логики для
         * последующего управления ими
         */
        Controller() noexcept;

        /**
         * @brief Универсальный метод для открытия файлов,
         * загруженных пользователем (кроме BMP изображений):
         * - CSV файл с выборкой
         * - Файл с весами перцептрона
         * @param path Путь до файла, который нужно считать
         * @note Если пользователь открывает файл с весами перцептрона,
         * то они автоматически загружаются в перцептрон
         */
        void open(const std::string& path);

        /**
         * @brief Запускает процесс тестирования работоспособности перцептрона
         * @param frac Показывает, на какой части выборки будет произведено тестирование,
         * где 0 - пустая выборка (вырожденная ситуация), а 1 - вся выборка целиком
         * @return Информация о том, насколько успешно прошло тестирование
         */
        SuccessRate testing(float frac) noexcept;

        /**
         * @brief Запускает процесс классификацию картинки перцептроном
         * @param data Данные пикселей, считанные из BMP картинки, загруженной пользователем
         * @return Буква, которая, по мнению перцептрона, изображена на картинке
         */
        char verify(const std::vector<float>& data) noexcept;

        /**
         * @brief Запуск процесса обучения с применением кросс-валидации
         * @param k Число групп, заданное пользователем
         * @return Информация о том, насколько успешно прошло обучение
         */
        SuccessRate crossValidation(int k) noexcept;

        /**
         * @brief Запускает обучение перцептрона
         * @param count_epoch Количество эпох, заданное пользователем
         */
        void training(int count_epoch) noexcept;

        /**
         * @brief Переключает реализацию перцептрона
         * @param type Новая реализация (Матричная или графовая)
         */
        void switchImplementation(PerceptronType type) noexcept;

        /**
         * @brief Переключает количество скрытых слоев перцептрона
         * @param count Новое количество (от 2 до 5)
         */
        void switchHiddenLayers(int count) noexcept;

        /**
         * @brief Сохраняет веса перцептрона в стандартный файл WEIGHTS_SAVE_FILE
         */
        void saveWeights() noexcept;

    private:
        std::unique_ptr<DataReader> data_reader_; ///< Считывает сохраненные веса перцептрона
        std::unique_ptr<EmnistDataReader> emnist_data_reader_; ///< Считывает выборки для тестирования/обучения перцептрона
        std::unique_ptr<Perceptron> perceptron_; ///< Модель перцептрона
    };
}

#endif
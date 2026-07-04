#ifndef PERCEPTRON_TOOLS_H
#define PERCEPTRON_TOOLS_H

/**
 * @file tools.h
 * @brief В этом файле находятся побочные классы, осуществляющие второстепенные действия
 * для обеспечения работоспособности всей программы,
 * такие как обеспечение связи между пользователем и программой через файловый I/O
 * @author Georgiy Kovalev
 */

#include <vector>
#include <string>

/**
 * @class BaseFileReader
 * @brief Абстрактный класс, описывающий минимальный функционал класса,
 * который бы выполнял действия, связанные с файлами
 */
class BaseFileReader {
public:
    /**
     * @brief Каждый класс-наследник должен обязательно иметь метод
     * для считывания данных из файла
     * @param path_to_file Путь до необходимого файла
     * @return Массив данных, который потом будет обработан программой
     * @warning Вид выходного массива данных может измениться в процессе разработки
     */
    virtual std::vector<float> Read(const std::string& path_to_file) = 0;

    virtual ~BaseFileReader() = default;
};

/**
 * @class DataReader
 * @brief Класс, ответственный за загрузку данных программы (весов перцептрона) и
 * сохранения их в отдельном файле
 */
class DataReader : public BaseFileReader {
public:

    /**
     * @brief Считывает веса перцептрона из файла для их последующей загрузки в программу
     * @param path_to_file Путь до файла с данными
     * @return Массив данных, который был считан из файла
     */
    std::vector<float> Read(const std::string& path_to_file) override;

    /**
     * @brief Сохраняет данные перцептрона в файл для того,
     * чтобы их загрузить после перезагрузки программы
     * @param path Путь, куда нужно сохранять файл
     * @param data Массив данных, которые необходимо сохранить
     */
    void saveData(const std::string& path, const std::vector<float>& data);
};

/**
 * @struct EmnistData
 * @brief Хранит данные для обучения/тестирования перцептрона
 */
struct EmnistData {
    EmnistData(const char index, const std::vector<float>& data) noexcept : index_(index), data_(data) {}

    const int& index() const noexcept { return index_; }
    const std::vector<float>& data() const noexcept { return data_; }

private:
    int index_; ///< Символ, который представлен в виде данной картинки
    std::vector<float> data_; ///< Нормализованные данные картинки в виде матрицы пикселей 26x26
};

/**
 * @class EmnistDataReader
 * @brief Класс, который считывает и обрабатывает emnist-letters данные из csv файла.
 * Необходим, когда нужно загрузить в перцептрон массив данных для обучения или тестирования
 */
class EmnistDataReader {
public:
    /**
     * @brief Считывает данные из csv файла, представляющие двумерный массив 26x26,
     * хранящий информацию о степени серости каждого пикселя (0 - черный, 255 - белый)
     * @param path_to_file Исходный файл с данными.
     * Внутри - N строк, в каждой из которых записаны через запятую числа от 0 до 255.
     * @return Массив данных "символ - пиксели", которые можно загрузить в перцептрон
     */
    std::vector<EmnistData> Read(const std::string& path_to_file);

private:
    /**
     * @brief Приводит данные к корректному виду, которые сможет обработать перцептрон
     * @param raw_data Исходные данные, считанные из файла. Какие преобразования необходимо применить к ним:
     * 1. Нормализовать (привести каждое значение к промежутку от 0 до 1)
     * 2. Инвертировать пиксели (первоначально картинка хранится в "негативе")
     * 3. Транспонировать матрицу ("изображение" повернуто на бок)
     * @return Данные, пригодные к загрузке в перцептрон
     */
    std::vector<float> Normalize(const std::vector<int>& raw_data) noexcept;
};

#endif

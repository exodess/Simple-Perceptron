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
    void saveData(const std::string& path, const std::vector<float>& data) noexcept;
};

/**
 * @class ImageReader
 * @brief Класс, который считывает загруженную картинку и преобразует ее в вид,
 * понятный программе (перцептрону)
 * @note Поддерживает картинки в формате BMP
 */
class ImageReader : public BaseFileReader {
public:
    /**
     * @brief При создании резервируем место для хранения результирующей матрицы в result_image_
     */
    ImageReader() noexcept;

    /**
     * @brief Считывает картинку
     * @param path_to_file Путь до картинки в формате BMP
     * @return Массив данных, считанный из картинки
     */
    std::vector<float> Read(const std::string& path_to_file) override;

    /**
     * @brief Позволяет пользователю посмотреть на сжатое изображение
     */
    void getImage(const std::string& output_file) noexcept;

private:
    /**
     * @brief Сохраняем в кэше изображение, которое получили на выходе,
     * на случай, если пользователь захочет посмотреть результат (нужно для отладки)
     * @note Размер стандартный - 28х28 пикселей
     */
    std::vector<float> result_image_;

    /**
     * @brief Приводит входную картинку к стандартному размеру 28х28 пикселей.
     * Для того, чтобы добится необходимого разрешения, необходимо заменить блок пикселей на
     * один, который получит среднее арифметическое значение всех пикселей, которых он заменил.
     * @param raw_data Данные, которые были считаны из файла с
     */
    void Resize(const std::vector<unsigned char>& raw_data, long old_image_width, long old_image_height) noexcept;
};

#endif

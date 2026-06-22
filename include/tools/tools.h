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
    virtual std::vector<int> Read(const std::string& path_to_file) = 0;

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
    std::vector<int> Read(const std::string& path_to_file) override;

    /**
     * @brief Сохраняет данные перцептрона в файл для того,
     * чтобы их загрузить после перезагрузки программы
     * @param path Путь, куда нужно сохранять файл
     * @param data Массив данных, которые необходимо сохранить
     */
    void saveData(const std::string& path, const std::vector<int>& data) noexcept;
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
     * @brief Считывает картинку
     * @param path_to_file Путь до картинки в формате BMP
     * @return Массив данных, считанный из картинки
     */
    std::vector<int> Read(const std::string& path_to_file) override;

private:
    /**
     * @brief Нормализует данные из картинки
     * @param data Массив данных в первичном виде, который будет преобразован
     */
    void Normalize(const std::vector<int>& data) noexcept;
};

#endif

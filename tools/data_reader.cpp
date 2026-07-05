#include "tools/tools.h"
#include <fstream>
#include <stdexcept>

namespace perc {
    void DataReader::Read(const std::string &path_to_file) {
        std::vector<float> result_data;

        std::ifstream file;
        file.open(path_to_file, std::ios::binary);

        if (!file.is_open()) {
#ifdef DEBUG
            std::cout << "Файл \"" << path_to_file << "\" не найден!\n";
#endif
            throw std::runtime_error("The file does not exist");
        }

        // Расчет количества элементов в файле
        file.seekg(0, std::ios::end);
        long count_element = file.tellg() / sizeof(float);
        result_data.reserve(count_element);

        const long step = 256; // Количество ячеек, считываемых за один раз
        char buffer[sizeof(float) * step];

        // Читаем по count значений float с начала файла
        file.seekg(std::ios::beg);
        while (count_element != 0) {
            // Количество ячеек float, которые считаются на этом шагу
            std::size_t count = std::min(step, count_element);
            file.read(buffer, count * sizeof(float));

            result_data.insert(result_data.end(), buffer, buffer + count * sizeof(float));

            count_element -= count;
        }

        file.close();

        data_ = result_data;
    }

    void DataReader::saveData(const std::string &path, const std::vector<float> &data) noexcept {
        std::ofstream file;
        file.open(path, std::ios::binary);

        if (!file.is_open()) {
#ifdef DEBUG
            std::cout << "Не удалось открыть файл \"" << path << "\"\n";
#endif
            throw std::runtime_error("Couldn't open the file");
        }

        const long step = 256; // Количество ячеек, считываемых за один раз
        long size_data = data.size();
        char buffer[sizeof(float) * step];
        auto it = data.begin();

        // Считываем по count значений float из вектора data
        while (size_data != 0) {
            std::size_t count = std::min(size_data, step);

            std::copy(it, it + count, buffer);
            file.write(buffer, count * sizeof(float));

            size_data -= count;
            it += count;
        }

        file.close();
    }
}
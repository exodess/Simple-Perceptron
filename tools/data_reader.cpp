#include "tools/tools.h"
#include <fstream>
#include <stdexcept>

std::vector<int> DataReader::Read(const std::string &path_to_file) {
    std::vector<int> result_data;

    std::ifstream file;
    file.open(path_to_file);

    if (!file.is_open()) {
#ifdef DEBUG
        std::cout << "Файл \"" << path_to_file << "\" не найден!\n";
#endif
        throw std::runtime_error("The file does not exist");
    }

    // Пока что примерный код, отвечающий за считывание данных
    while (!file.eof()) {
        int value;
        file >> value;
        result_data.push_back(value);
    }

    file.close();
    return result_data;
}

void DataReader::saveData(const std::string &path, const std::vector<int> &data) noexcept {
    std::ofstream file;
    file.open(path);

    if (!file.is_open()) {
#ifdef DEBUG
        std::cout << "Не удалось открыть файл \"" << path << "\"\n";
#endif
        throw std::runtime_error("Couldn't open the file");
    }

    // Тоже примерный код для сохранения массива данных
    for (auto v : data) {
        file << v;
    }

    file.close();
}
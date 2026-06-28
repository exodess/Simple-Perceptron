#include "tools/tools.h"
#include "data/data.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<EmnistData> EmnistDataReader::Read(const std::string &path_to_file) {
    std::vector<EmnistData> result_data;

    std::ifstream file;
    file.open(path_to_file);

    if (!file.is_open()) {
#ifdef DEBUG
        std::cout << "Файл \"" << path_to_file << "\" не найден!\n";
#endif
        throw std::runtime_error("The file does not exist");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> raw_data;
        std::stringstream line_stream(line);
        std::string num;

        while (std::getline(line_stream, num, ',')) {
            raw_data.push_back(static_cast<char>(std::stoi(num)));
        }

        // Первое число - номер символа, который нарисован на этой картинке
        char number = raw_data[0];
        raw_data.erase(raw_data.begin());

        if (raw_data.size() != DEFAULT_WIDTH_RES * DEFAULT_HEIGHT_RES) {
            std::cout << "Количество пикселей отличается от стандартного!" << std::endl;
            continue;
        }

        // Преобразуем данные в корректный вид
        auto normalize_data = Normalize(raw_data);

        result_data.push_back(EmnistData(number - 1, normalize_data));
    }

    file.close();

    return result_data;
}

std::vector<float> EmnistDataReader::Normalize(const std::vector<char> &raw_data) noexcept {
    std::vector<float> result_data;
    result_data.reserve(DEFAULT_WIDTH_RES * DEFAULT_HEIGHT_RES); // В Read() уже есть проверка по размеру матрицы

    // Нормализуем и инвертируем данные
    for (auto i = 0; i < DEFAULT_WIDTH_RES * DEFAULT_WIDTH_RES; ++i) {
        result_data.push_back(static_cast<float>(raw_data[i]) / 255.0f);
        result_data[i] = 1.0f - result_data[i];
    }

    // Транспонируем матрицу
    for (auto i = 0; i < DEFAULT_WIDTH_RES; ++i) {
        for (auto j = i; j < DEFAULT_HEIGHT_RES; ++j) {
            std::swap(result_data[i * DEFAULT_WIDTH_RES + j], result_data[j * DEFAULT_WIDTH_RES + i]);
        }
    }

    return result_data;
}


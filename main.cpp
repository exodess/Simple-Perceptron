#include "include/tools/tools.h"
#include "include/data/data.h"
#include "include/perceptron/perceptron.h"
#include <iostream>

int main() {
    EmnistDataReader csv_reader;
    DataReader dataReader;


    std::cout << "Hello, World!" << std::endl;

    try {
        auto res = csv_reader.Read("materials/emnist/emnist-letters-test.csv");
        std::cout << "EmnistDataReader: Считано " << res.size() << " примеров\n";
        std::cout << "Первое изображение: " << static_cast<char>(res[5].letter() + 'a' - 1) << "\n";

        auto result_image = res[5].data();

        for (auto i = 0; i < 28; ++i) {
            for (auto j = 0; j < 28; ++j) {
                float pixel = result_image[i * 28 + j];
                char symbol = ' ';

                if (pixel < 1.0f && pixel >= 0.7f) symbol = '\'';
                else if (pixel < 0.7f && pixel >= 0.5f) symbol = '+';
                else if (pixel < 0.5f && pixel >= 0.3f) symbol = '*';
                else if (pixel < 0.3f && pixel >= 0.0f) symbol = '#';

                // std::cout << symbol << symbol;
            }
            // std::cout << std::endl;
        }

        unique_ptr<Perceptron> perceptron = Perceptron::create(MATRIX_VIEW, 0, 2);
        dataReader.Read("data.txt");
        perceptron->loadWeights(dataReader.Read("data.txt"));
        perceptron->training(1, res);
        // dataReader.saveData("data.txt", perceptron->saveWeights());

    } catch (const std::exception& e) {
        std::cout << "Не удалось обработать изображение!\n";
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}

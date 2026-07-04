#include "include/tools/tools.h"
#include "include/data/data.h"
#include "include/perceptron/perceptron.h"
#include "include/perceptron/graph_perceptron.h"
#include "include/perceptron/matrix_perceptron.h"
#include <iostream>
#include <memory>



static std::unique_ptr<perc::Perceptron> create(perc::PerceptronType type, int size) 
{
    if (type == perc::MATRIX_VIEW)
        return std::make_unique<perc::Matrix_perceptron>(size);
    return std::make_unique<perc::Graph_perceptron>(size);
}

int main() {
    EmnistDataReader csv_reader;
    DataReader dataReader;


    std::cout << "Hello, World!" << std::endl;

    try {
        auto res = csv_reader.Read("materials/emnist/emnist-letters-test.csv");
        std::cout << "EmnistDataReader: Считано " << res.size() << " примеров\n";
        std::cout << "Первое изображение: " << static_cast<char>(res[5].index() + 'a' - 1) << "\n";

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

        std::unique_ptr<perc::Perceptron> perceptron = create(perc::GRAPH_VIEW, 2);
        // dataReader.Read("data.txt");
        // perceptron->loadWeights(dataReader.Read("data.txt"));
        for (int i = 0; i < 15; ++i) {
            perceptron->Train(res);
            std::cout << "~~~~~~\n";
        }
        // dataReader.saveData("data.txt", perceptron->saveWeights());

    } catch (const std::exception& e) {
        std::cout << "Не удалось обработать изображение!\n";
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}

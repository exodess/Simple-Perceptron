#include "include/tools/tools.h"
#include "include/data/data.h"
#include "include/perceptron/perceptron.h"
#include "include/perceptron/graph_perceptron.h"
#include "include/perceptron/matrix_perceptron.h"
#include <iostream>
#include <memory>
#include <random>
#include <ranges>
#include <algorithm>



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

                // std::cout << pixel << " ";
            }
            // std::cout << std::endl;
        }

        std::unique_ptr<perc::Perceptron> perceptron = create(perc::MATRIX_VIEW, 2);

        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(res.begin(), res.end(), g);

        for (int i = 0; i < 5; ++i) {
            perceptron->Train(res);
        }

        int accuracy{};
        std::shuffle(res.begin(), res.end(), g);

        for (auto& x : res) {
            if (perceptron->Verify(x.data()) == x.index() - 1)
                accuracy++;
        }

        std::cout << "accuracy is " << accuracy;

    } catch (const std::exception& e) {
        std::cout << "Не удалось обработать изображение!\n";
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}

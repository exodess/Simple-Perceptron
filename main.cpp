#include "tools/tools.h"
#include <iostream>

int main() {
    ImageReader bmp_reader;
    EmnistDataReader csv_reader;
    std::cout << "Hello, World!" << std::endl;

    try {
        bmp_reader.Read("../materials/convertico-abstract-design-1-bit-black-and-white-bmp.bmp");
        bmp_reader.getImage("out.bmp");

        auto res = csv_reader.Read("../materials/emnist/emnist-letters-test.csv");
        std::cout << "EmnistDataReader: Считано " << res.size() << " примеров\n";
        std::cout << "Первое изображение: " << static_cast<char>(res[0].letter() + 'a' - 1) << "\n";

        auto result_image = res[0].data();

        for (auto i = 0; i < 28; ++i) {
            for (auto j = 0; j < 28; ++j) {
                float pixel = result_image[i * 28 + j];
                char symbol = ' ';

                if (pixel < 1.0f && pixel >= 0.7f) symbol = '\'';
                else if (pixel < 0.7f && pixel >= 0.5f) symbol = '+';
                else if (pixel < 0.5f && pixel >= 0.3f) symbol = '*';
                else if (pixel < 0.3f && pixel >= 0.0f) symbol = '#';

                std::cout << symbol << symbol;
            }
            std::cout << std::endl;
        }

    } catch (const std::exception& e) {
        std::cout << "Не удалось обработать изображение!\n";
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}

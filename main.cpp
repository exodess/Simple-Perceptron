#include "tools/tools.h"
#include <iostream>

int main() {
    ImageReader bmp_reader;
    std::cout << "Hello, World!" << std::endl;

    try {
        bmp_reader.Read("../materials/convertico-abstract-design-1-bit-black-and-white-bmp.bmp");
        bmp_reader.getImage("out.bmp");
    } catch (const std::exception& e) {
        std::cout << "Не удалось обработать изображение!\n";
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}

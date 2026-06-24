#include <iostream>

#include "tools/tools.h"

int main() {
    ImageReader bmp_reader;
    std::cout << "Hello, World!" << std::endl;

    bmp_reader.Read("../materials/sample-bmp-files-sample_640x426.bmp");
    return 0;
}

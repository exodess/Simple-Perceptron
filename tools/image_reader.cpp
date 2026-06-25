#include "tools/tools.h"
#include "tools/bitmap.h"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <iostream>

#define DEBUG

std::vector<int> ImageReader::Read(const std::string &path_to_file) {
    using namespace perc_bmp;

    // Необходимые данные для обработки изображения
    DWORD image_width, image_height;
    WORD bits_per_pixel;
    DWORD compression;
    DWORD image_size = 0;
    DWORD red_bitmask, green_bitmask, blue_bitmask, alpha_bitmask;

    std::ifstream file;
    file.open(path_to_file, std::ios::binary);

    if (!file.is_open()) {
#ifdef DEBUG
        std::cout << "Не удалось открыть файл \"" << path_to_file << "\"\n";
#endif
        throw std::runtime_error("The file does not exist");
    }

    // Считывание заголовочной структуры
    char bitmapfileheader_buffer[sizeof(BITMAPFILEHEADER)]; // Буфер структуры заголовка
    BITMAPFILEHEADER bitmapfileheader;

    file.read(bitmapfileheader_buffer, sizeof(BITMAPFILEHEADER));
    memcpy(&bitmapfileheader, bitmapfileheader_buffer, sizeof(BITMAPFILEHEADER));

    std::cout << "Считана структура BITMAPFILEHEADER" << std::endl;
    std::cout << "\tПроверка сигнатуры... ";
    if (bitmapfileheader.bfType == BITMAP_SIGNATURE) {
        std::cout << "+\n";
    } else {
        std::cout << "несоответствие формата: " << bitmapfileheader.bfType << " (должно быть 4d42)\n";

        throw std::runtime_error("Incorrect file format");
    }

#ifdef DEBUG
    std::cout << "\tРазмер файла: " << bitmapfileheader.bfSize << " байт" << std::endl;
    std::cout << "\tПиксельные данные находятся по смещению: " << std::hex << bitmapfileheader.bfOffBits << std::endl;
#endif

    // Считывание размера информационной структуры
    BITMAPCOREHEADER bitmapcoreheader;
    char bitmapcoreheader_buffer[sizeof(BITMAPCOREHEADER)];

    file.read(bitmapcoreheader_buffer, sizeof(BITMAPCOREHEADER));
    memcpy(&bitmapcoreheader, bitmapcoreheader_buffer, sizeof(BITMAPCOREHEADER));
    DWORD bitmapinfo_version = bitmapcoreheader.bcSize;

    std::cout << "Считана CORE версия структуры BITMAPINFO" << std::endl;
    std::cout << "Вычисление версии BITMAPINFO... ";
    if (bitmapinfo_version == sizeof(BITMAPCOREHEADER)) {
        std::cout << "CORE\n";

        image_width = bitmapcoreheader.bcWidth;
        image_height = bitmapcoreheader.bcHeight;
        bits_per_pixel = bitmapcoreheader.bcBitCount;

    } else if (bitmapinfo_version == sizeof(BITMAPINFOHEADER)) {
        std::cout << "3\n";
        std::cout << "Считывание остальной части BITMAPINFOHEADER структуры... ";

        // Передвигаем указатель в файле на начало структуры
        file.seekg(sizeof(BITMAPFILEHEADER), std::ios::beg);

        BITMAPINFOHEADER bitmapinfoheader;
        char bitmapinfoheader_buffer[sizeof(BITMAPINFOHEADER)];

        file.read(bitmapinfoheader_buffer, sizeof(BITMAPINFOHEADER));
        memcpy(&bitmapinfoheader, bitmapinfoheader_buffer, sizeof(BITMAPINFOHEADER));

        image_width = bitmapinfoheader.biWidth;
        image_height = bitmapinfoheader.biHeight;
        bits_per_pixel = bitmapinfoheader.biBitCount;
        compression = bitmapinfoheader.biCompression;
        image_size = bitmapinfoheader.biSizeImage;

        std::cout << "+" << std::endl;

    } else if (bitmapinfo_version == sizeof(BITMAPV4HEADER)) {
        std::cout << "4\n";
        std::cout << "Считывание остальной части BITMAPV5HEADER структуры... ";

        // Передвигаем указатель в файле на начало структуры
        file.seekg(sizeof(BITMAPFILEHEADER), std::ios::beg);

        BITMAPV4HEADER bitmapv4header;
        char bitmapv4header_buffer[sizeof(BITMAPV4HEADER)];

        file.read(bitmapv4header_buffer, sizeof(BITMAPV4HEADER));
        memcpy(&bitmapv4header, bitmapv4header_buffer, sizeof(BITMAPV4HEADER));

        image_width = bitmapv4header.bV4Width;
        image_height = bitmapv4header.bV4Height;
        bits_per_pixel = bitmapv4header.bV4BitCount;
        compression = bitmapv4header.bV4Compression;
        image_size = bitmapv4header.bV4SizeImage;
        red_bitmask = bitmapv4header.bV4RedMask;
        green_bitmask = bitmapv4header.bV4GreenMask;
        blue_bitmask = bitmapv4header.bV4BlueMask;

    } else if (bitmapinfo_version == sizeof(BITMAPV5HEADER)) {
        std::cout << "5\n";
        std::cout << "Считывание остальной части BITMAPV5HEADER структуры... ";

        file.seekg(sizeof(BITMAPFILEHEADER), std::ios::beg);
        BITMAPV5HEADER bitmapv5header;
        char bitmapv5header_buffer[sizeof(BITMAPV5HEADER)];

        file.read(bitmapv5header_buffer, sizeof(BITMAPV5HEADER));
        memcpy(&bitmapv5header, bitmapv5header_buffer, sizeof(BITMAPV5HEADER));

        image_width = bitmapv5header.bV5Width;
        image_height = bitmapv5header.bV5Height;
        bits_per_pixel = bitmapv5header.bV5BitCount;
        compression = bitmapv5header.bV5Compression;
        image_size = bitmapv5header.bV5SizeImage;
        red_bitmask = bitmapv5header.bV5RedMask;
        green_bitmask = bitmapv5header.bV5GreenMask;
        blue_bitmask = bitmapv5header.bV5BlueMask;

        std::cout << "+" << std::endl;

    } else {
        throw std::runtime_error("Unknown version of the BITMAPINFO structure");
    }

#ifdef DEBUG
    std::cout << "\tШирина изображения: " << std::dec << image_width << std::endl;
    std::cout << "\tВысота изображения: " << std::dec << image_height << std::endl;
    std::cout << "\tРазрядность: " << bits_per_pixel << " бит" << std::endl;
    std::cout << "\tПоле compression: " << std::hex << compression << std::endl;
#endif

    // Считываем непосредственно пиксельные данные
    file.seekg(bitmapfileheader.bfOffBits, std::ios::beg);
    for (int i = 0; i < 10; ++i) {
        DWORD pixel;
        char pixel_buffer[bits_per_pixel / 8];

        file.read(pixel_buffer, sizeof(pixel_buffer));
        memcpy(&pixel, pixel_buffer, sizeof(pixel_buffer));

        std::cout << "Считан " << (i + 1) << "-ый пиксель\n";
        std::cout << "\tКрасный канал: " << std::hex << ((pixel & red_bitmask) >> 16) << std::endl;
        std::cout << "\tЗеленый канал: " << std::hex << ((pixel & green_bitmask) >> 8) << std::endl;
        std::cout << "\tСиний канал: " << std::hex << (pixel & blue_bitmask) << std::endl;
    }

    file.close();

    return {};
}

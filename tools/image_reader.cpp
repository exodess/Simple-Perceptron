#include "tools/tools.h"
#include "tools/bitmap.h"
#include "data/data.h"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <iostream>

#define DEBUG

ImageReader::ImageReader() noexcept {
    result_image_.reserve(DEFAULT_WIDTH_RES * DEFAULT_HEIGHT_RES);
}

std::vector<int> ImageReader::Read(const std::string &path_to_file) {
    using namespace perc_bmp;

    // Необходимые данные для обработки изображения
    DWORD image_width, image_height; // Разрешение изображения
    DWORD image_size; // Размер пиксельных данных в байтах
    WORD bits_per_pixel;

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
        image_size = bitmapfileheader.bfSize - bitmapfileheader.bfOffBits;
        bits_per_pixel = (image_size * 8) / (image_width * image_height);

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
        image_size = bitmapv4header.bV4SizeImage;

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
        image_size = bitmapv5header.bV5SizeImage;

        std::cout << "+" << std::endl;

    } else {
        throw std::runtime_error("Unknown version of the BITMAPINFO structure");
    }

#ifdef DEBUG
    std::cout << "\tШирина изображения: " << std::dec << image_width << std::endl;
    std::cout << "\tВысота изображения: " << std::dec << image_height << std::endl;
#endif

    // Проверка перед считыванием данных
    if (bits_per_pixel > 1) {
        throw std::runtime_error("Unsupported bit depth");
    }
    // if (image_height > MAX_HEIGHT_RES || image_width > MAX_WIDTH_RES) {
    //     throw std::runtime_error("Unsupported image resolution");
    // }

    std::cout << "Чтение пиксельных данных... ";

    std::vector<BYTE> pixel_data_buffer;
    pixel_data_buffer.reserve(image_width * image_height);

    // Считываем непосредственно пиксельные данные
    file.seekg(bitmapfileheader.bfOffBits, std::ios::beg);
    for (int i = 0; i < image_size / sizeof(DWORD); ++i) {
        DWORD pixel = 0;
        char pixel_buffer[sizeof(pixel)];

        file.read(pixel_buffer, sizeof(pixel_buffer));
        memcpy(&pixel, pixel_buffer, sizeof(pixel_buffer));

        // Каждый бит - отдельное значение пикселя
        for (int j = sizeof(DWORD) * 8 - 1; j >= 0; --j) {
            BYTE value = ((pixel >> j) & 1);
            pixel_data_buffer.push_back(value);
        }
    }

    std::cout << "+\n";

    Resize(pixel_data_buffer, image_width, image_height);

    file.close();

    return result_image_;
}

void ImageReader::getImage(const std::string& output_file) noexcept {
    // Пока что изображение выводится на экран
    std::cout << "Создается сжатое изображение 28x28... ";
    std::cout << "+\n";

    for (auto i = 0; i < DEFAULT_HEIGHT_RES; ++i) {
        for (auto j = 0; j < DEFAULT_WIDTH_RES; ++j) {
            char symbol = (result_image_[i * DEFAULT_HEIGHT_RES + j] == 0) ? '#' : ' ';

            std::cout << "[" << symbol << "]";
        }
        std::cout << std::endl;
    }
}

void ImageReader::Resize(const std::vector<unsigned char>& raw_data, long old_image_width, long old_image_height) noexcept {
    // В результате должно получится массив размером 28x28 пикселей

    perc_bmp::DWORD width = old_image_width, height = old_image_height;
    std::vector<float> pixel_coef;
    pixel_coef.reserve(old_image_width * old_image_height);

    for (int i = 0; i < old_image_height * old_image_width; ++i) {
        pixel_coef[i] = static_cast<float>(raw_data[i]);
    }

    while (height != DEFAULT_HEIGHT_RES || width != DEFAULT_WIDTH_RES) {
        if (height != DEFAULT_HEIGHT_RES) {
            perc_bmp::DWORD new_height = height;

            for (auto i = 0; i < height - 1; ++i) {
                for (auto j = 0; j < width; ++j) {
                    float res_pixel = (pixel_coef[i * width + j] + pixel_coef[(i + 1) * width + j]) / 2.0f;

                    pixel_coef[i * width + j] = res_pixel;
                }

                if (--new_height == DEFAULT_HEIGHT_RES) {
                    break;
                }
            }

            // Удаляем лишние горизонтальные линии в матрице
            for (auto i = 0; i < (height - new_height); --height) {
                perc_bmp::DWORD begin_index = (i + 1) * width;
                perc_bmp::DWORD end_index = (i + 2) * width;

                pixel_coef.erase(pixel_coef.begin() + begin_index, pixel_coef.begin() + end_index);
            }
        }

        if (width != DEFAULT_WIDTH_RES) {
            perc_bmp::DWORD new_width = width;

            for (auto j = 0; j < width - 1; ++j) {
                for (auto i = 0; i < height; ++i) {
                    float res_pixel = (pixel_coef[i * width + j] + pixel_coef[i * width + j + 1]) / 2.0f;

                    pixel_coef[i * width + j] = res_pixel;
                }

                if (--new_width == DEFAULT_WIDTH_RES) {
                    break;
                }
            }

            // Удаляем лишние вертикальные линии в матрице
            for (auto j = 0; j < (width - new_width); --width) {
                for (auto i = 0; i < height; ++i) {
                    // учитываем смещение после того, как удалится очередной элемент
                    pixel_coef.erase(pixel_coef.begin() + (i * width + j + 1 - i));
                }
            }
        }
    }

    // Копируем результат в "кэш" класса
    for (int i = 0; i < DEFAULT_HEIGHT_RES * DEFAULT_WIDTH_RES; ++i) {
        int value = (pixel_coef[i] <= 0.5f) ? 0 : 1;

        result_image_[i] = value;
    }

}

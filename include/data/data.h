#ifndef PERCEPTRON_DATA_H
#define PERCEPTRON_DATA_H

/**
 * @file data.h
 * @brief В этом файле содержатся основные константы, используемые в программе
 * @author Georgiy Kovalev
 */

#define MAX_WIDTH_RES 512 ///< Максимальная ширина изображения, которую может обработать программа
#define MAX_HEIGHT_RES 512 ///< Максимальная высота изображения, которую может обработать программа
#define DEFAULT_WIDTH_RES 28 ///< Ширина изображения, которое подается в перцептрон.
#define DEFAULT_HEIGHT_RES 28 ///< Высота изображения, которое подается в перцептрон.

#define COUNT_LETTERS 26

// Настройки перцептрона
#define INPUT_SIZE DEFAULT_WIDTH_RES * DEFAULT_HEIGHT_RES
#define OUTPUT_SIZE COUNT_LETTERS
#define MIN_K 1
#define DEFAULT_K 5 ///< Значение k для кросс-валидации
#define MAX_K 20
#define MIN_COUNT_EPOCHS 10
#define DEFAULT_COUNT_EPOCHS 30
#define MAX_COUNT_EPOCHS 120
#define MIN_HIDDEN 2 ///< Минимальное количество скрытых слоев перцептрона
#define MAX_HIDDEN 5 ///< Максимальное количество скрытых слоев перцептрона
#define MAX_NEURONS 64
#define LEARNING_RATE 0.1f

// Настройки GUI
#define DEFAULT_WINDOW_WIDTH 900
#define DEFAULT_WINDOW_HEIGHT 600
#define MINIMUM_WINDOW_WIDTH 600
#define MINIMUM_WINDOW_HEIGHT 600

#define WEIGHTS_SAVE_FILE ".weights"

#endif
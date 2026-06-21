#ifndef CLASS_STRUCTURE_H
#define CLASS_STRUCTURE_H

// Здесь представленно примерное описание классов, как их вижу я
// Они были описаны в файле plan.md

#define MAX_WIDTH_RES 512
#define MAX_HEIGHT_RES 512

// ==============================
// ========== TOOLS =============
// ==============================

class ImageReader {
public:
    // Считывание изображения, которое находится по конкретному пути path
    // Возвращает нормализованную матрицу (максимальный размер - 512*512)
    // Если будет ошибка с файлом, то выбросит исключение
    std::vector<std::vector<uint8_t>> readImage(const std::string& path);

private:
    // Нормализация матрицы
    void Normalize() noexcept;
};

class DataReader {
public:
    // Загрузка данных о весах из конкретного файла
    std::vector<int> readData(const std::string& path);

    // Сохранение данных о весах в конкретный файл
    void saveData(const std::vector<int>& data, const std::string& path) noexcept;

};

class Neuron {
public:
    explicit Neuron(char letter) noexcept; // Создание нейрона с привязанной к нему буквой
    explicit Neuron(int number) noexcept; // Создание нейрона с порядковым номером буквы в алфавите

    // Функция активации
    int sigmoidalFunc(int x);

private:
    char letter_; // буква, с которой ассоциируется нейрон
    int input_[MAX_WIDTH_RES][MAX_HEGHT_RES]
    int output_; // результат работы нейрона
    int memory_[MAX_WIDTH_RES][MAX_HEGHT_RES];

};

// Слой представляет из себя N паралелльных несвязанных между собой нейронов
class Layer {
public:
    Layer(int count_neurons) noexcept;

private:
    std::vector<Neuron> neurons_;
};

enum PerceptronType {
    MATRIX_VIEW,
    GRAPH_VIEW
};

class Perceptron {
public:
    // Создание перцептрона определенного вида
    Perceptron(PerceptronType type);

    // Идентифицирует букву по входным данным, полученным из ImageReader'а
    char identify(std::vector<std::vector<uint8_t>> init_data) noexcept;

    // обучение на открытом датасете
    void training(Dataset data);

    // Задание нового количества скрытых слоев
    void setCountLayer(int value) noexcept;

private:
    PerceptronType type_;
    std::vector<Layout> inner_layouts_; // Внутренние слои (от 2 до 5)

};

#endif

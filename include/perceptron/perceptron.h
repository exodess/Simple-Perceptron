#include <vector>
#include <memory>

#define MAX_WIDTH_RES 512
#define MAX_HEIGHT_RES 512
#define INPUT_SIZE  784
#define OUTPUT_SIZE 26
#define MIN_HIDDEN  2
#define MAX_HIDDEN  5

using std::vector;
using std::unique_ptr;

enum PerceptronType {
    MATRIX_VIEW,
    GRAPH_VIEW
};

class Perceptron {
public:
    static unique_ptr<Perceptron> create(PerceptronType type, int number, int hidden_layers) noexcept;

    // Идентифицирует букву по входным данным, полученным из ImageReader'а
    char identify(std::vector<std::vector<uint8_t>> init_data) noexcept;

    // обучение на открытом датасете
    void training(Dataset data);

    // Задание нового количества скрытых слоев
    void setCountLayer(int value) noexcept;

    virtual void backPropagation();

    virtual void sumFunc();

    virtual int predict();
};
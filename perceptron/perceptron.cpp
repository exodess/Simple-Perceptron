#include <../include/perceptron/perceptron.h>
#include <../include/perceptron/matrix_perceptron.h>
#include <../include/perceptron/graph_perceptron.h>
#include <memory>

unique_ptr<Perceptron> create(PerceptronType type, int number, int hidden_layers) noexcept {
    if (type == MATRIX_VIEW)
        return std::make_unique<Matrix_perceptron>(number, hidden_layers);
    // return std::make_unique<Graph_perceptron>(number, hidden_layers);
}
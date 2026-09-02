#include <gtest/gtest.h>
#include "perceptron/graph_perceptron.h"
#include "perceptron/matrix_perceptron.h"
#include "tools/tools.h"

TEST(DataReaderTest, test_1) {
    perc::Graph_perceptron perceptron(2);
    perc::DataReader reader;

    auto init_weigths = perceptron.GetWeights();

    reader.saveData(".temp", init_weigths);

    reader.Read(".temp");
    auto save_data = reader.data();

    EXPECT_EQ(init_weigths.size(), save_data.size());
    EXPECT_EQ(init_weigths == save_data, true);
}

TEST(DataReaderTest, test_2) {
    perc::Matrix_perceptron perceptron(2);
    perc::DataReader reader;

    auto init_weigths = perceptron.GetWeights();

    reader.saveData(".temp", init_weigths);

    reader.Read(".temp");
    auto save_data = reader.data();

    EXPECT_EQ(init_weigths.size(), save_data.size());
    EXPECT_EQ(init_weigths == save_data, true);
}
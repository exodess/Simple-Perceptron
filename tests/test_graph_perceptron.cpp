#include <gtest/gtest.h>
#include "perceptron/graph_perceptron.h"
#include "tools/tools.h"

using namespace perc;

TEST(GraphPerceptronTest, test_1) {
    Graph_perceptron p(2);
    std::vector<float> test_weights = p.GetWeights();
    EXPECT_EQ(p.hiddenLayers(), 2);
    for (auto& x : test_weights) {
        EXPECT_GE(x, -0.5f);
        EXPECT_LT(x, 0.51f);
    }
}

TEST(GraphPerceptronTest, test_2) {
    Graph_perceptron p(2);
    std::vector<float> image(784, 0.5f);
    int result = p.Verify(image);
    EXPECT_GE(result, 0);
    EXPECT_LT(result, 26);
}

TEST(GraphPerceptronTest, test_3) {
    Graph_perceptron p(2);
    std::vector<float> image(784, 0.5f);
    int result = p.Verify(image);
    std::vector<float> test_weights_1 = p.GetWeights();

    p.Reset();

    std::vector<float> test_weights_2 = p.GetWeights();
    EXPECT_NE(test_weights_1, test_weights_2);
}

TEST(GraphPerceptronTest, test_4) {
    Graph_perceptron p(2);
    auto test_weights_1 = p.GetWeights();
    p.LoadWeights(test_weights_1);
    EXPECT_EQ(p.GetWeights(), test_weights_1);
}

TEST(GraphPerceptronTest, test_5) {

    std::vector<float> image(784, 0.5f);
    std::vector<EmnistData> data;
    data.push_back(EmnistData{1, image});
    data.push_back(EmnistData{1, image});

    Graph_perceptron p(2);
    float result = p.Train(data);

    EXPECT_GE(result, -0.0f);
}
#include "perceptron/graph_perceptron.h"

namespace perc {

Graph_perceptron::Graph_perceptron(int hidden_layers_count) noexcept: Perceptron(GRAPH_VIEW, hidden_layers_count) {
    
    std::vector<int> layer_sizes;
    layer_sizes.push_back(INPUT_SIZE);
    
    int size = 32;

    for (int i = hidden_layers_count - 1; i >= 0; --i) {
        layer_sizes.insert(layer_sizes.begin() + 1, size * (1 << i));
    }

    layer_sizes.push_back(COUNT_LETTERS);
    layers_.resize(layer_sizes.size());
    
    for (int i = 0; i < layers_.size(); ++i) {
        int prev_size = (i == 0) ? 0 : layer_sizes[i - 1];
        layers_[i] = Graph_Layer{prev_size, layer_sizes[i]};
    }

    int total_edges = 0;
    for (int i = 0; i < layers_.size() - 1; ++i)
        total_edges += layers_[i].neurons_.size() * layers_[i + 1].neurons_.size();
    all_edges_.reserve(total_edges);
        
    for (int i = 0; i < layers_.size() - 1; ++i) {
        Graph_Layer& layer = layers_[i];
        Graph_Layer& next_layer = layers_[i + 1];
        
        for (auto& neuron : layer.neurons_) {
            neuron.bias_ = 0.0;
            for (auto& next_neuron : next_layer.neurons_) {
                next_neuron.bias_ = 0.0;
                float weight = setRandomWeight();
                
                all_edges_.push_back(Edge{&neuron, &next_neuron, weight});
                Edge* edge = &all_edges_.back();
                neuron.outputs_.push_back(edge);
                next_neuron.inputs_.push_back(edge);
            }
        }
    }
}

float Graph_perceptron::setRandomWeight() noexcept {
    static std::random_device rd;
    static  std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.5, 0.5);

    return dist(gen);
}

void Graph_perceptron::Reset() noexcept {
        
    for (int i = 0; i < layers_.size() - 1; ++i) {
        Graph_Layer& layer = layers_[i];
        Graph_Layer& next_layer = layers_[i + 1];
        int prev_size = layer.neurons_.size();
        
        for (auto& neuron : layer.neurons_) {
            neuron.bias_ = 0.0;
        }
        
        for (auto& next_neuron : next_layer.neurons_) {
            next_neuron.bias_ = 0.0;
        }

        for (auto& edge : all_edges_) {
            edge.weight_ = setRandomWeight();
        }
    }
}

void Graph_perceptron::sumFunc(const std::vector<float>& normalize_input) noexcept {
    int i{};

    for (auto& neuron : layers_[0].neurons_)
        neuron.output_ = normalize_input[i++];

    for (int l = 1; l < layers_.size(); ++l) {
        Graph_Layer& layer = layers_[l];

        for (int i = 0; i < layer.neurons_.size(); ++i) {
            Graph_Neuron& neuron = layer.neurons_[i];
            float sum = neuron.bias_;
            for (auto& input : neuron.inputs_)
                sum += input->from_->output_ * input->weight_;
            neuron.output_ = neuron.sigmoidalFunc(sum);
        }

    }
}

void Graph_perceptron::updateWeights() noexcept {
    for (int l = 1; l < layers_.size(); ++l) {
        Graph_Layer& layer = layers_[l];

        for (int i = 0; i < (int)layer.neurons_.size(); ++i) {
            auto& neuron = layer.neurons_[i];
                for (auto& input : neuron.inputs_) {
                    input->weight_ -=  LEARNING_RATE * neuron.deltas_ * input->from_->output_;
                }
        }
    }
}

float Graph_perceptron::backPropagation(float expected_[OUTPUT_SIZE]) noexcept {

    Graph_Layer& layer = layers_.back();

    //обработка выходного слоя
    for (int i = 0; i < layer.neurons_.size(); ++i) {
        Graph_Neuron& neuron_ = layer.neurons_[i];
        float o = neuron_.output_;
        neuron_.deltas_ = (o - expected_[i]) * o * (1. - o);
        
    }

    //обработка слоев с конца
    for (int i = hidden_layers_count_; i > -1; i--) {
        Graph_Layer& layer_current = layers_[i];
        Graph_Layer& layer_next = layers_[i + 1];
        
        for (int g = 0; g < layer_current.neurons_.size(); ++g) {
            Graph_Neuron& neuron = layer_current.neurons_[g];
            float error = 0.0;

            for (auto& output : neuron.outputs_) {
                error += output->to_->deltas_ * output->weight_;
            }
            neuron.deltas_ = error * neuron.output_ * (1.0 - neuron.output_);
        }
    }

    //обновление весов
    updateWeights();

    //считаем ошибку для конкретного примера (вектора значений), чтобы высчитать ошибку (или потерю) для эпохи
    float total_error_{};
    for (int i = 0; i < layer.neurons_.size(); ++i) {
        Graph_Neuron& neuron_ = layer.neurons_[i];
        float diff_ = neuron_.output_ - expected_[i];
        total_error_+= diff_ * diff_;
    }

    return total_error_;
}

int Graph_perceptron::Verify(const std::vector<float>& image) noexcept  {
    sumFunc(image);

    Graph_Layer& last_layer_ = layers_[layers_.size() - 1];
    int best_index_{};
    float max_output_{last_layer_.neurons_[0].output_};

    for (int i = 0; i < last_layer_.neurons_.size(); ++i) {
        Graph_Neuron& neuron_ = last_layer_.neurons_[i];
        if (neuron_.output_ > max_output_) {
            best_index_ = i;
            max_output_ = neuron_.output_;
        }
    }
    return best_index_;
}

float Graph_perceptron::Train(const std::vector<EmnistData>& data) noexcept {

    float epoch_loss{};

    for (auto& input : data) {
        
        float y_training[OUTPUT_SIZE]{};

        y_training[input.index()] = 1;

        sumFunc(input.data());
        epoch_loss += backPropagation(y_training);
    }

    return epoch_loss;
}

void Graph_perceptron::LoadWeights(const std::vector<float>& data) noexcept {
    for (int i = 0; i < all_edges_.size(); ++i)
        all_edges_[i].weight_ = data[i];
}

std::vector<float> Graph_perceptron::GetWeights() noexcept {
    std::vector<float> weights{};

    for (int i = 0; i < all_edges_.size(); ++i)
        weights.push_back(all_edges_[i].weight_);

    return weights;
}

}
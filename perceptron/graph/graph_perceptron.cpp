#include "../../include/perceptron/graph_perceptron.h"

namespace perc {

Graph_perceptron::Graph_perceptron(int hidden_layers_count) noexcept: Perceptron(GRAPH_VIEW, hidden_layers_count),
    hidden_layers_count_{hidden_layers_count}, layers_count{hidden_layers_count + 2} {
    
    layers_.resize(layers_count);
    
    vector<int> layer_sizes;
    layer_sizes.push_back(INPUT_SIZE);
    
    int size = 64;
    for (int i = 0; i < hidden_layers_count; ++i) {
        size = size * 2;  // 128, 256, ...
        layer_sizes.push_back(size);
    }
    layer_sizes.push_back(COUNT_LETTERS);
    
    for (int i = 0; i < layers_count; ++i) {
        int prev_size = (i == 0) ? 0 : layer_sizes[i-1];
        int curr_size = layer_sizes[i];
        layers_[i] = Graph_Layer{prev_size, curr_size};
    }

    int total_edges = 0;
    for (int i = 0; i < layers_count - 1; ++i)
        total_edges += layers_[i].neurons_.size() * layers_[i + 1].neurons_.size();
    all_edges_.reserve(total_edges);
        
    for (int i = 0; i < layers_count - 1; ++i) {
        Graph_Layer& layer = layers_[i];
        Graph_Layer& next_layer = layers_[i + 1];
        int prev_size = layer.neurons_.size();
        
        for (auto& neuron : layer.neurons_) {
            neuron.bias_ = 0.0;
        }
        
        for (auto& next_neuron : next_layer.neurons_) {
            next_neuron.bias_ = 0.0;
        }
        
        for (auto& neuron : layer.neurons_) {
            for (auto& next_neuron : next_layer.neurons_) {
                float limit = sqrt(6.0 / prev_size);
                float weight = ((float)rand() / RAND_MAX) * 2 * limit - limit;
                
                all_edges_.push_back(Edge{&neuron, &next_neuron, weight});
                Edge* edge = &all_edges_.back();
                neuron.outputs_.push_back(edge);
                next_neuron.inputs_.push_back(edge);
            }
        }
    }
}

Graph_perceptron::Graph_perceptron(int hidden_layer_sizes, vector<float>& weights) noexcept: Perceptron(GRAPH_VIEW, hidden_layer_sizes),
    hidden_layers_count_{hidden_layer_sizes}, layers_count{hidden_layer_sizes + 2} {
    
    layers_.resize(layers_count);
    
    vector<int> layer_sizes;
    layer_sizes.push_back(INPUT_SIZE);
    int size = 64;
    for (int i = 0; i < hidden_layer_sizes; ++i) {
        size = (i < MAX_HIDDEN) ? 64 : 32;
        layer_sizes.push_back(size);
    }
    layer_sizes.push_back(COUNT_LETTERS);
    
    for (int i = 0; i < layers_count; ++i) {
        int prev_size = (i == 0) ? 0 : layer_sizes[i-1];
        int curr_size = layer_sizes[i];
        layers_[i] = Graph_Layer{prev_size, curr_size};
    }
    
    int total_edges = 0;
    for (int i = 0; i < layers_count - 1; ++i) {
        total_edges += layers_[i].neurons_.size() * layers_[i + 1].neurons_.size();
    }
    all_edges_.reserve(total_edges);
    
    for (auto& layer : layers_) {
        for (auto& neuron : layer.neurons_) {
            neuron.bias_ = 0.0f;
        }
    }
    
    int w = 0;
    for (int i = 0; i < layers_count - 1; ++i) {
        Graph_Layer& layer = layers_[i];
        Graph_Layer& next_layer = layers_[i + 1];
        
        for (auto& neuron : layer.neurons_) {
            for (auto& next_neuron : next_layer.neurons_) {
                all_edges_.push_back(Edge{&neuron, &next_neuron, weights[w]});
                Edge* edge = &all_edges_.back();
                neuron.outputs_.push_back(edge);
                next_neuron.inputs_.push_back(edge);
                w++;
            }
        }
    }
}

void Graph_perceptron::setDataInput(const vector<float>& normalize_input) noexcept {
    normalize_input_ = normalize_input;
}

void Graph_perceptron::sumFunc() noexcept {
    int i{};
    for (auto& neuron : layers_[0].neurons_)
        neuron.output_ = normalize_input_[i++];

    for (int l = 1; l < layers_count; ++l) {
        Graph_Layer& layer = layers_[l];

        // #pragma omp parallel for
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
    for (int l = 1; l < layers_count; ++l) {
        Graph_Layer& layer = layers_[l];

        // #pragma omp parallel for
        for (int i = 0; i < (int)layer.neurons_.size(); ++i) {
            auto& neuron = layer.neurons_[i];
                for (auto& input : neuron.inputs_) {
                    input->weight_ -=  learning_rate_ * neuron.deltas_ * input->from_->output_;
                }
        }
    }
}

float Graph_perceptron::backPropagation(float expected_[OUTPUT_SIZE]) noexcept {

    Graph_Layer& layer = layers_.back();

    //обработка выходного слоя
    // #pragma omp parallel for
    for (int i = 0; i < layer.neurons_.size(); ++i) {
        Graph_Neuron& neuron_ = layer.neurons_[i];
        float o = neuron_.output_;
        neuron_.deltas_ = (o - expected_[i]) * o * (1. - o);
        
    }

    //обработка слоев с конца
    for (int i = layers_count - 2; i > -1; i--) {
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

int Graph_perceptron::Verify(const vector<float>& image) noexcept  {
    setDataInput(image);
    sumFunc();

    Graph_Layer& last_layer_ = layers_[layers_count - 1];
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

        y_training[input.index() - 1] = 1;

        setDataInput(input.data());

        sumFunc();
        epoch_loss += backPropagation(y_training);
    }

    return epoch_loss;
}

void Graph_perceptron::LoadWeights(const vector<float>& data) noexcept {
    for (int i = 0; i < all_edges_.size(); ++i)
        all_edges_[i].weight_ = data[i];
}

vector<float> Graph_perceptron::GetWeights() noexcept {
    vector<float> weights{};

    for (int i = 0; i < all_edges_.size(); ++i)
        weights.push_back(all_edges_[i].weight_);

    return weights;
}

}
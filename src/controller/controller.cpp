#include "controller/controller.h"
#include "perceptron/matrix_perceptron.h"
#include "perceptron/graph_perceptron.h"
#include <data/data.h>
#include <map>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <random>
#include <algorithm>

namespace perc {

    Controller::Controller() noexcept
    : data_reader_(std::make_unique<DataReader>())
    , emnist_data_reader_(std::make_unique<EmnistDataReader>())
    , perceptron_(std::make_unique<Matrix_perceptron>(MIN_HIDDEN)) {}

    void Controller::open(const std::string& path) {
        std::filesystem::path file(path);

        if (file.extension().string() == ".csv") {
            emnist_data_reader_->Read(path);
        } else if (file.extension().string() == WEIGHTS_SAVE_FILE_EXTENSION) {
            data_reader_->Read(path);
        } else {
            throw std::runtime_error("The file could not be recognized: " + file.filename().string());
        }
    }

    SuccessRate Controller::testing(float frac) noexcept {
        std::random_device rd;
        std::mt19937 g(rd());

        auto data = emnist_data_reader_->data();
        std::ranges::shuffle(data.begin(), data.end(), g);
        data.erase(data.begin(), data.begin() + data.size() * (1.0 - frac));

        return TestDataset(data);
    }

    SuccessRate Controller::TestDataset(std::vector<EmnistData> data) noexcept {
        SuccessRate result;
        int count_correct_res = 0;
        std::map<int, int> correct_particular_res;
        std::map<int, int> particular_res;

        // Начинаем проверку и замеряем время
        auto begin_time = std::chrono::steady_clock::now();
        int total_size = data.size();
        for (int i = 0; i < total_size; ++i) {
            // Берем рандомный датасет из списка
            int rand_emnist = std::rand() % data.size();

            auto res = perceptron_->Verify(data[rand_emnist].data());
            auto correct_res = data[rand_emnist].index();

            count_correct_res += (res == correct_res);
            correct_particular_res[correct_res] += (res == correct_res);
            particular_res[correct_res] ++;

            // Удаляем использованный датасет
            data.erase(data.begin() + rand_emnist);
        }
        // Фиксируем конец проверки
        auto end_time = std::chrono::steady_clock::now();

        if (total_size > 0) {
            // Считаем долю правильных ответов
            result.accuracy() = static_cast<float>(count_correct_res) / total_size;

            // Считаем среднюю долю правильных ответов по классам
            float sum_precisions = 0.0f;
            for (auto m : correct_particular_res) {
                sum_precisions += static_cast<float>(m.second) / particular_res[m.first];
            }
            result.precision() = sum_precisions / correct_particular_res.size();

            // Расчет recall (пока что приравниваем к precision)
            result.recall() = result.precision();

            // Считаем f-measure
            result.f_measure() = 2.0f * result.precision() * result.recall() / (result.precision() + result.recall());

            // Считаем затраченное время
            result.time() = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count();
        }

        return result;
    }

    char Controller::verify(const std::vector<float> &data) noexcept {
        int res_index = perceptron_->Verify(data);

        return (res_index != -1) ? static_cast<char>(res_index + 'a') : '?';
    }

    SuccessRate Controller::crossValidation(int k) noexcept {
        SuccessRate result;
        std::random_device rd;
        std::mt19937 g(rd());
        auto data = emnist_data_reader_->data();

        // Перемешиваем случайным образом выборку и делим на k частей
        std::ranges::shuffle(data.begin(), data.end(), g);
        int part = data.size() / k; // Размер одной части исходного датасета

        for (auto i = 0; i < k; ++i) {
            std::vector<EmnistData> test_dataset(part);
            auto train_dataset = data;
            std::move(train_dataset.begin() + i * part, train_dataset.begin() + (i + 1) * part, test_dataset.begin());
            train_dataset.erase(train_dataset.begin() + i * part, train_dataset.begin() + (i + 1) * part);

            perceptron_->Reset(); // Перцептрон по умолчанию
            perceptron_->Train(train_dataset); // Тренируем его на k-ой части датасета
            auto rate = TestDataset(test_dataset); // Проверяем на остальной части выборки

            result += rate;
        }

        return result / k;
    }

    std::vector<float> Controller::training(int count_epoch) noexcept {
        auto data = emnist_data_reader_->data();
        std::vector<float> error_values(count_epoch);

        if (data.size()) {
            std::random_device rd;
            std::mt19937 g(rd());
            std::ranges::shuffle(data.begin(), data.end(), g);

            for (auto i = 0; i < count_epoch; ++i) {

                auto res = perceptron_->Train(data);
                error_values[i] = res;
            }
        }
        else {
            error_values = std::vector<float>(count_epoch, 1.0f);
        }

        return error_values;
    }

    void Controller::switchImplementation(PerceptronType type) noexcept {
        // Сохраняем количество скрытых слоев
        int count_hidden = perceptron_->hiddenLayers();

        if (perceptron_->type() != type) {
            // Сохраняем веса перцептрона
            auto weights_data = perceptron_->GetWeights();

            // Создаем перцептрон с таким же количеством скрытых слоев
            if (type == MATRIX_VIEW) {
                perceptron_ = std::make_unique<Matrix_perceptron>(count_hidden);
            } else {
                perceptron_ = std::make_unique<Graph_perceptron>(count_hidden);
            }

            // Загружаем веса обратно
            perceptron_->LoadWeights(weights_data);
        }
    }

    void Controller::switchHiddenLayers(int count) noexcept {
        // Проверка количества скрытых слоев идет в GUI
        if (count != perceptron_->hiddenLayers()) {
            if (perceptron_->type() == MATRIX_VIEW) {
                perceptron_ = std::make_unique<Matrix_perceptron>(count);
            } else {
                perceptron_ = std::make_unique<Graph_perceptron>(count);
            }
        }
    }

    void Controller::saveWeights(const std::string& dest) noexcept {
        auto data = perceptron_->GetWeights();

        data_reader_->saveData(dest, data);
    }

}


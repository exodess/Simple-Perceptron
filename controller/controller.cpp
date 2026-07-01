#include "controller/controller.h"
#include <map>
#include <chrono>
#include <iostream>
#include <filesystem>

namespace perc {

    Controller::Controller() noexcept
    : data_reader_(std::make_unique<DataReader>())
    , emnist_data_reader_(std::make_unique<EmnistDataReader>())
    , perceptron_(std::make_unique<Perceptron>(MATRIX_VIEW)) {}

    void Controller::open(const std::string& path) {
        std::filesystem::path file(path);

        if (file.extension().c_str() == ".csv") {
            emnist_data_reader_->Read(path);
        } else if (path == WEIGHTS_SAVE_FILE) {
            data_reader_->Read(path);
        } else {
            throw std::runtime_error("The file could not be recognized: " + file.filename().string());
        }
    }

    SuccessRate Controller::testing(float frac) noexcept {
        SuccessRate result;

        auto data = emnist_data_reader_->data();
        int size_data = static_cast<int>(data.size() * frac);
        int count_correct_res = 0;
        std::map<int, int> correct_particular_res;
        std::map<int, int> particular_res;

        // Начинаем проверку и замеряем время
        auto begin_time = std::chrono::steady_clock::now();
        for (int i = 0; i < size_data; ++i) {
            // Берем рандомный датасет из списка
            int rand_emnist = std::rand() % data.size();

            auto res = perceptron_->identify(data[rand_emnist].data());
            auto correct_res = data[rand_emnist].index();

            count_correct_res += (res == correct_res);
            correct_particular_res[correct_res] += (res == correct_res);
            particular_res[correct_res] ++;

            // Удаляем использованный датасет
            data.erase(data.begin() + rand_emnist);
        }
        // Фиксируем конец проверки
        auto end_time = std::chrono::steady_clock::now();

        if (size_data > 0) {
            // Считаем долю правильных ответов
            result.accuracy() = static_cast<float>(count_correct_res / size_data);

            // Считаем среднюю долю правильных ответов по классам
            float sum_precisions = 0.0f;
            for (auto m : correct_particular_res) {
                sum_precisions += static_cast<float>(m.second / particular_res[m.first]);
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
        int res_index = perceptron_->predict(data);

        return (res_index != -1) ? static_cast<char>(res_index + 'a') : '?';
    }

    SuccessRate Controller::crossValidation(int k) noexcept {
        SuccessRate result;
        auto data = emnist_data_reader_->data();

        return result;
    }

    void Controller::training(int count_epoch) noexcept {
        auto data = emnist_data_reader_->data();

        perceptron_->training(count_epoch, data);
    }

    void Controller::switchImplementation(PerceptronType type) noexcept {
        perceptron_->type() = type;
    }

    void Controller::switchHiddenLayers(int count) noexcept {
        // Проверка количества скрытых слоев идет в GUI
        perceptron_->hiddenLayer() = count;
    }

    void Controller::saveWeights() noexcept {
        auto& data = perceptron_->weghts();

        data_reader_->saveData(WEIGHTS_SAVE_FILE, data);
    }

}


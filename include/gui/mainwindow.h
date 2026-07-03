#ifndef PERCEPTRON_MAIN_WINDOW_H
#define PERCEPTRON_MAIN_WINDOW_H

#include "gui_mainwindow.h"
#include "controller/controller.h"
#include <QMainWindow>

namespace gui {
    /**
     * @class MainWindow
     * @brief Главное окно приложения.
     * Компоновка: вертикальная панель кнопок-навигации слева -
     * каждая кнопка переключает соответствующую страницу содержимого:
     * 1. Страница "Эксперимент".
     * Здесь происходит загрузка тестовой выборки и запуск тестирования на ней
     *
     */
    class MainWindow : public QMainWindow, public GUI_MainWindow {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override = default;

    private slots:
        /**
         * @brief Порядок страниц в m_contentStack совпадает с порядком кнопок:
         * @param index номер страницы справа: 0 - эксперимент, 1 - обучение, 2 - загрузить
         */
        void on_btn_NavigationButton_clicked(int index) noexcept;
        void on_btn_LoadTestSample_clicked() noexcept; ///< Обработка нажатия на кнопку загрузки тестовой выборки

        // ==============================
        // === Страница "Эксперимент" ===
        // ==============================

        void on_slider_Sample_valueChanged() noexcept; ///< Обработка ползунка слайдера
        void on_btn_StartTesting_clicked() noexcept; ///< Обработка нажатия на кнопку запуска эксперимента на текущей тестовой выборке

        // ==============================
        // ===== Страница "Обучение" ====
        // ==============================

        void on_btn_SubNavigationButton_clicked(int index) noexcept; ///< Обработка нажатия на кнопки навигации на странице "Обучение"

        void on_spin_CountEpochs_valueChanged() noexcept; ///< Обработка спинбокса для задания количества эпох обучения

        void on_btn_LoadTrainSample_clicked() noexcept;
        /**
         * @brief Обработка нажатия кнопки "Начать обучение"
         * на странице "Обучение" - "Обычное обучение"
         */
        void on_btn_StartNormalTraining_clicked() noexcept;
        void on_spin_CrossValidationK_valueChanged() noexcept; ///< Обработка спинбокса для задания значения k

        /**
         * @brief Обработка нажатия кнопки "Начать обучение"
         * на странице "Обучение" - "Обучение с кросс-валидацией"
         */
        void on_btn_StartCrossValidationTraining_clicked() noexcept;

        // ==============================
        // ==== Страница "Настройки" ====
        // ==============================

        void on_combo_PerceptronType_indexChanged(int index) noexcept; ///< Выбор реализации перцептрона
        void on_spin_CountHiddenLayers_valueChanged() noexcept; ///< Выбор количества скрытых слоев перцептрона
        void on_btn_SaveWeights_clicked() noexcept; ///< Обработка нажатия на кнопку сохранения весов перцептрона в отдельном файле
        void on_btn_LoadWeights_clicked() noexcept; ///< Обработка нажатия на кнопку загрузки весов перцептрона из файла

    private:
        void connectSignals() const noexcept; ///< Соединяет все слоты с сигналами в приложении

        std::unique_ptr<GUI_MainWindow> ui_; ///< Управление пользовательским интерфейсом
        std::unique_ptr<perc::Controller> controller_; ///< Контроллер для управления всей внутренней логикой программы
        QString current_test_sample_; ///< Название файла, откуда загружается тестовая выборка
        float part_sample_; ///< Какая часть тестовой выборки будет загружена
        int k_; ///< Значение k для обучения методом кросс-валидации
        int count_epochs_; ///< Количество эпох обучения перцептрона
    };
}

#endif
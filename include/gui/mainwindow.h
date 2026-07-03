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
        void on_btn_NavigationButton_clicked(int index); ///< Нажатие на соответсвующую кнопку навигации
        void on_btn_LoadSample_clicked(); ///< Обработка нажатия на кнопку загрузки тестовой выборки
        void on_btn_StartSample_clicked(); ///< Обработка нажатия на кнопку запуска эксперимента на текущей тестовой выборке

        void on_slider_Sample_valueChanged();

    private:
        void connectSignals() noexcept; ///< Соединяет все слоты с сигналами в приложении

        std::unique_ptr<GUI_MainWindow> ui_; ///< Управление пользовательским интерфейсом
        std::unique_ptr<perc::Controller> controller_;
        QString current_test_sample_; ///< Название файла, откуда загружается тестовая выборка
        float part_sample_; ///< Какая часть тестовой выборки будет загружена

    };
}

#endif
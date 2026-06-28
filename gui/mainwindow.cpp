#include "gui/mainwindow.h"
#include <data/data.h>
#include <QFileDialog>
#include <QMainWindow>

namespace gui {
    MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_{std::make_unique<GUI_MainWindow>()}
    , part_sample_(1.0f)
    {
        setWindowTitle("Перцептрон");
        resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
        setMinimumSize(MINIMUM_WINDOW_WIDTH, MINIMUM_WINDOW_HEIGHT);

        ui_->setupUI();
        setCentralWidget(ui_->widget_CentralWidget);

        connectSignals();
        qDebug() << "[MainWindow] Инициализация завершена";
    }

    void MainWindow::connectSignals() noexcept {
        connect(ui_->group_NavigationButtons, &QButtonGroup::idClicked, this, &MainWindow::on_btn_NavigationButton_clicked);
        connect(ui_->btn_LoadSample, &QPushButton::clicked, this, &MainWindow::on_btn_LoadSample_clicked);
        connect(ui_->btn_StartSample, &QPushButton::clicked, this, &MainWindow::on_btn_StartSample_clicked);

        // Ползунок
        connect(ui_->slider_Sample, &QSlider::valueChanged, this, &MainWindow::on_slider_Sample_valueChanged);
    }

    void MainWindow::on_slider_Sample_valueChanged() {
        part_sample_ = static_cast<float>(ui_->slider_Sample->value()) / 100.0;

        qDebug() << "Установлено значение на слайдере: " << part_sample_;
    }

    void MainWindow::on_btn_LoadSample_clicked() {
        QString fileName = QFileDialog::getOpenFileName(
            this, "Открыть тестовую выборку", QString(),
            "CVS Files (*.cvs);;All Files (*)");

        if (!fileName.isEmpty()) {
            qDebug() << "Загружен файл " << fileName;
            current_test_sample_ = fileName;

            ui_->slider_Sample->setVisible(true);
        }
    }

    void MainWindow::on_btn_StartSample_clicked() {
        if (!current_test_sample_.isEmpty()) {
            qDebug() << "Начало прогона тестовой выборки\n";
        }
    }

    void MainWindow::on_btn_NavigationButton_clicked(int index){
        ui_->widget_ContentStack->setCurrentIndex(index);
    }

}
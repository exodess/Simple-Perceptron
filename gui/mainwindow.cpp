#include "gui/mainwindow.h"
#include "data/data.h"
#include <QFileDialog>
#include <QMainWindow>

namespace gui {
    MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_{std::make_unique<GUI_MainWindow>()}
    , controller_(std::make_unique<perc::Controller>())
    , part_sample_(1.0f)
    , k_(DEFAULT_K)
    , count_epochs_(DEFAULT_COUNT_EPOCHS)
    {
        setWindowTitle("Перцептрон");
        resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
        setMinimumSize(MINIMUM_WINDOW_WIDTH, MINIMUM_WINDOW_HEIGHT);

        ui_->setupUI();
        setCentralWidget(ui_->widget_CentralWidget);

        connectSignals();
        qDebug() << "[MainWindow] Инициализация завершена";
    }

    void MainWindow::connectSignals() const noexcept {
        // Соединяем все сигналы

        connect(ui_->group_NavigationButtons, &QButtonGroup::idClicked, this, &MainWindow::on_btn_NavigationButton_clicked);

        connect(ui_->btn_LoadTestSample, &QPushButton::clicked, this, &MainWindow::on_btn_LoadTestSample_clicked);
        connect(ui_->slider_TestSample, &QSlider::valueChanged, this, &MainWindow::on_slider_Sample_valueChanged);
        connect(ui_->btn_StartTesting, &QPushButton::clicked, this, &MainWindow::on_btn_StartTesting_clicked);
        connect(ui_->btn_LoadImage, &QPushButton::clicked, this, &MainWindow::on_btn_LoadImage_clicked);

        connect(ui_->group_TrainNav, &QButtonGroup::idClicked, this, &MainWindow::on_btn_SubNavigationButton_clicked);
        connect(ui_->spin_EpochsGroups, &QSpinBox::valueChanged, this, &MainWindow::on_spin_CountEpochs_valueChanged);
        connect(ui_->btn_LoadTrainSample, &QPushButton::clicked, this, &MainWindow::on_btn_LoadTrainSample_clicked);
        connect(ui_->btn_StartNormalTrain, &QPushButton::clicked, this, &MainWindow::on_btn_StartNormalTraining_clicked);
        connect(ui_->spin_KGroups, &QSpinBox::valueChanged, this, &MainWindow::on_spin_CrossValidationK_valueChanged);
        connect(ui_->btn_StartCrossValTrain, &QPushButton::clicked, this, &MainWindow::on_btn_StartCrossValidationTraining_clicked);

        connect(ui_->combo_ImplMenu, &QComboBox::currentIndexChanged, this, &MainWindow::on_combo_PerceptronType_indexChanged);
        connect(ui_->spin_HiddenLayers, &QSpinBox::valueChanged, this, &MainWindow::on_spin_CountHiddenLayers_valueChanged);
        connect(ui_->btn_SaveWeights, &QPushButton::clicked, this, &MainWindow::on_btn_SaveWeights_clicked);
        connect(ui_->btn_LoadWeights, &QPushButton::clicked, this, &MainWindow::on_btn_LoadWeights_clicked);
    }

    void MainWindow::on_btn_NavigationButton_clicked(int index) noexcept {
        ui_->widget_ContentStack->setCurrentIndex(index);
    }

    void MainWindow::on_btn_LoadTestSample_clicked() noexcept {
        QString fileName = QFileDialog::getOpenFileName(
            this, "Открыть тестовую выборку", QString(),
            "CSV Files (*.csv);;All Files (*)");

        if (!fileName.isEmpty()) {
            qDebug() << "Загружен файл " << fileName;

            current_test_sample_ = fileName;
            controller_->open(fileName.toStdString());

            ui_->slider_TestSample->setVisible(true);
            ui_->label_TestSampleStatus->setVisible(true);
            ui_->label_TestSampleStatus->setText("Загружен файл " + QFileInfo(fileName).fileName());
        }
    }

    void MainWindow::on_slider_Sample_valueChanged() noexcept {
        part_sample_ = static_cast<float>(ui_->slider_TestSample->value()) / 100.0;
    }

    void MainWindow::on_btn_StartTesting_clicked() noexcept {
        if (!current_test_sample_.isEmpty()) {
            qDebug() << "Начало прогона тестовой выборки\n";

            auto res = controller_->testing(part_sample_);

            // Выводим на экран информацию из res
            ui_->label_TestAccuracy->setText(QString("Average accuracy: %1").arg(res.accuracy()));
            ui_->label_TestPrecision->setText(QString("Precision: %1").arg(res.precision()));
            ui_->label_TestRecall->setText(QString("Recall: %1").arg(res.recall()));
            ui_->label_TestFMeasure->setText(QString("F-measure: %1").arg(res.recall()));
            ui_->label_TestTime->setText(QString("Time spent (sec): %1").arg(res.time() / 1000.0f));

            qDebug() << "Тестирование завершено";
        }
    }

    void MainWindow::on_btn_LoadImage_clicked() noexcept {
        QString file_path = QFileDialog::getOpenFileName(
            this, "Открыть изображение", QString(),
            "Изображения (*.png *.jpg *.bmp)");

        if (!file_path.isEmpty()) {
            QPixmap pix(file_path);
            if (!pix.isNull()) {
                // Масштабируем строго до 28х28 согласно условию
                ui_->label_ImagePreview->setPixmap(pix.scaled(28, 28, Qt::KeepAspectRatio, Qt::FastTransformation));
                ui_->label_ImagePreview->setVisible(true);
            }
        }
    }

    void MainWindow::on_btn_SubNavigationButton_clicked(int index) noexcept {
        ui_->widget_TrainStack->setCurrentIndex(index);
    }

    void MainWindow::on_spin_CountEpochs_valueChanged() noexcept {
        count_epochs_ = ui_->spin_EpochsGroups->value();
    }

    void MainWindow::on_btn_LoadTrainSample_clicked() noexcept {
        QString fileName = QFileDialog::getOpenFileName(
            this, "Открыть тренировочную выборку", QString(),
            "CSV Files (*.csv);;All Files (*)");

        if (!fileName.isEmpty()) {
            qDebug() << "Загружен файл " << fileName;

            current_test_sample_ = fileName;
            controller_->open(fileName.toStdString());

            ui_->label_TrainSampleStatus->setVisible(true);
            ui_->label_TrainSampleStatus->setText("Загружен файл " + QFileInfo(fileName).fileName());
        }
    }

    void MainWindow::on_btn_StartNormalTraining_clicked() noexcept {
        qDebug() << "Начинается обучение перцептрона...";

        auto graphic_data = controller_->training(count_epochs_);

        // Построение графика
        qDebug() << "Обучение завершено";
    }

    void MainWindow::on_spin_CrossValidationK_valueChanged() noexcept {
        k_ = ui_->spin_KGroups->value();
    }

    void MainWindow::on_btn_StartCrossValidationTraining_clicked() noexcept {
        auto res = controller_->crossValidation(k_);

        // Вывод метрик из res на экран
    }

    void MainWindow::on_combo_PerceptronType_indexChanged(int index) noexcept {
        perc::PerceptronType type;

        if (index == 0) {
            type = perc::MATRIX_VIEW;

            qDebug() << "Выбрана матричная реализация перцептрона";
        }
        else {
            type = perc::GRAPH_VIEW;

            qDebug() << "Выбрана графовая реализация перцептрона";
        }

        controller_->switchImplementation(type);
    }

    void MainWindow::on_spin_CountHiddenLayers_valueChanged() noexcept {
        int count = ui_->spin_HiddenLayers->value();

        controller_->switchHiddenLayers(count);
    }

    void MainWindow::on_btn_SaveWeights_clicked() noexcept {
        QString fileName = QFileDialog::getOpenFileName(
            this, "Выберите файл для сохранения весов", QString(),
            "All Files (*)");

        if (!fileName.isEmpty()) {
            if ("." + QFileInfo(fileName).suffix() != WEIGHTS_SAVE_FILE_EXTENSION) {
                fileName += WEIGHTS_SAVE_FILE_EXTENSION;
            }

            qDebug() << "Веса сохранены в файле " << fileName;

            current_test_sample_ = fileName;
            controller_->saveWeights(fileName.toStdString());
        }
    }

    void MainWindow::on_btn_LoadWeights_clicked() noexcept {
        QString fileName = QFileDialog::getOpenFileName(
            this, "Открыть файл с весами", QString(),
            "Weights File (*.weights);;All Files (*)");

        if (!fileName.isEmpty()) {
            qDebug() << "Загружен файл " << fileName;

            current_test_sample_ = fileName;
            controller_->open(fileName.toStdString());
        }
    }

}
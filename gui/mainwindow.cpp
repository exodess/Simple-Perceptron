#include "gui/mainwindow.h"
#include "data/data.h"
#include "gui/drawing_dialog.h"
#include <QFileDialog>
#include <QMainWindow>
#include <algorithm>

namespace gui {
    MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_{std::make_unique<GUI_MainWindow>()}
    , controller_(std::make_unique<perc::Controller>())
    , part_sample_(DEFAULT_PART_VALUE)
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
        connect(ui_->btn_DrawLetter, &QPushButton::clicked, this, &MainWindow::on_btn_DrawImage_clicked);
        connect(ui_->btn_Identify, &QPushButton::clicked, this, &MainWindow::on_btn_IdentifyLetter_clicked);

        connect(ui_->group_TrainNav, &QButtonGroup::idClicked, this, &MainWindow::on_btn_SubNavigationButton_clicked);
        connect(ui_->spin_EpochsGroups, &QSpinBox::valueChanged, this, &MainWindow::on_spin_CountEpochs_valueChanged);
        connect(ui_->btn_LoadNormalTrainSample, &QPushButton::clicked, this, &MainWindow::on_btn_LoadTrainSample_clicked);
        connect(ui_->btn_StartNormalTrain, &QPushButton::clicked, this, &MainWindow::on_btn_StartNormalTraining_clicked);
        connect(ui_->spin_KGroups, &QSpinBox::valueChanged, this, &MainWindow::on_spin_CrossValidationK_valueChanged);
        connect(ui_->btn_LoadValidationTrainSample, &QPushButton::clicked, this, &MainWindow::on_btn_LoadTrainSample_clicked);
        connect(ui_->btn_StartCrossValTrain, &QPushButton::clicked, this, &MainWindow::on_btn_StartCrossValidationTraining_clicked);

        connect(ui_->combo_ImplMenu, &QComboBox::currentIndexChanged, this, &MainWindow::on_combo_PerceptronType_indexChanged);
        connect(ui_->spin_HiddenLayers, &QSpinBox::valueChanged, this, &MainWindow::on_spin_CountHiddenLayers_valueChanged);
        connect(ui_->btn_SaveWeights, &QPushButton::clicked, this, &MainWindow::on_btn_SaveWeights_clicked);
        connect(ui_->btn_LoadWeights, &QPushButton::clicked, this, &MainWindow::on_btn_LoadWeights_clicked);
    }

    void MainWindow::drawTrainGraph(const std::vector<float>& graph) noexcept {
        if (graph.empty()) return;

        ui_->chartView_Training->setVisible(true);

        // Формируем список точек для быстрой замены данных
        QList<QPointF> points;
        points.reserve(graph.size());
        for (size_t i = 0; i < graph.size(); ++i) {
            points.append(QPointF(static_cast<double>(i), graph[i]));
        }

        // Накладываем новые точки на уже существующий в UI график
        ui_->series_Training->replace(points);

        // Динамически пересчитываем границы осей под новые данные
        ui_->axisX_Training->setRange(0, graph.size() - 1);

        auto [min_it, max_it] = std::minmax_element(graph.begin(), graph.end());
        float min_val = *min_it;
        float max_val = *max_it;

        // Добавляем 5% свободного пространства сверху и снизу для красоты
        float offset = (max_val - min_val) * 0.05f;
        if (offset == 0.0f) offset = 0.1f; // Защита от деления на 0 / одинаковых значений

        ui_->axisY_Training->setRange(min_val - offset, max_val + offset);
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

            ui_->slider_TestSample->setVisible(true);
            ui_->label_TestSampleStatus->setVisible(true);
            ui_->label_TestSampleStatus->setText("Загружен файл " + QFileInfo(fileName).fileName());
            ui_->label_GlobalStatus->setText("Выбрана тестовая выборка");
        }
    }

    void MainWindow::on_slider_Sample_valueChanged() noexcept {
        part_sample_ = static_cast<float>(ui_->slider_TestSample->value()) / 100.0;
    }

    void MainWindow::on_btn_StartTesting_clicked() noexcept {
        qDebug() << "Загружается файл с тестовой выборкой:" << current_test_sample_;

        try {
            ui_->label_GlobalStatus->setText("Загружаем файл выборки в программу");
            ui_->label_GlobalStatus->repaint();

            controller_->open(current_test_sample_.toStdString());

            qDebug() << "Начало прогона тестовой выборки";

            ui_->label_GlobalStatus->setText("Идет тестирование перцептрона...");
            ui_->label_GlobalStatus->repaint();
            auto res = controller_->testing(part_sample_);

            // Выводим на экран информацию из res
            ui_->label_TestAccuracy->setText(QString("Average accuracy: %1").arg(res.accuracy()));
            ui_->label_TestPrecision->setText(QString("Precision: %1").arg(res.precision()));
            ui_->label_TestRecall->setText(QString("Recall: %1").arg(res.recall()));
            ui_->label_TestFMeasure->setText(QString("F-measure: %1").arg(res.recall()));
            ui_->label_TestTime->setText(QString("Time spent (sec): %1").arg(res.time() / 1000.0f));
            ui_->widget_TestMetricsCenter->setVisible(true);

            ui_->label_GlobalStatus->setText("Тестирование завершено");
        }
        catch (const std::exception& e) {
            ui_->label_GlobalStatus->setText(e.what());
        }

        qDebug("Завершено");
    }

    void MainWindow::on_btn_LoadImage_clicked() noexcept {
        QString fileName = QFileDialog::getOpenFileName(
            this, "Открыть изображение", QString(),
            "Изображения (*.png *.jpg *.bmp)");

        if (!fileName.isEmpty()) {
            qDebug() << "Загружен файл " << fileName;

            drawLetter(fileName);

            ui_->label_GlobalStatus->setText("Изображение сохранено");
        }
    }

    void MainWindow::drawLetter(const QString &path_image) noexcept {
        // Загружаем изображение
        QImage img(path_image);
        if (img.isNull()) {
            ui_->label_GlobalStatus->setText("Не удалось загрузить изображение");
            return;
        }

        // Конвертируем в 8-битные оттенки серого (1 байт на пиксель, 0-255)
        img = img.convertToFormat(QImage::Format_Grayscale8);

        // Масштабируем строго до необходимо размера
        img = img.scaled(DEFAULT_WIDTH_RES, DEFAULT_HEIGHT_RES, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        image_ = img;

        // Загружаем BMP-файл в QPixmap
        QPixmap pixmap(path_image);

        // Проверяем, корректно ли загрузился файл
        if (!pixmap.isNull()) {
            // Рисуем квадратной формы
            int w = pixmap.width();
            int h = pixmap.height();
            int side = qMin(w, h); // Берем меньшую сторону

            int x = (w - side) / 2;
            int y = (h - side) / 2;

            // Сохраняем идеальный квадрат в переменную класса
            pixmap = pixmap.copy(x, y, side, side);

            // 2. МАСШТАБИРОВАНИЕ ПОД ТЕКУЩИЙ РАЗМЕР
            // Вычисляем доступный размер внутри рамки QLabel (минус 2 пикселя на границы)
            QSize display_size = ui_->label_ImagePreview->size() - QSize(2, 2);

            // Масштабируем с сохранением пропорций и жесткими границами пикселей
            pixmap = pixmap.scaled(
                display_size,
                Qt::KeepAspectRatio,
                Qt::FastTransformation
            );

            // Отображаем. QLabel сам отцентрирует этот квадрат внутри себя
            ui_->label_ImagePreview->setPixmap(pixmap);
            ui_->label_IdentifyResult->setVisible(false);
        }
    }


    void MainWindow::on_btn_DrawImage_clicked() noexcept {
        ui_->label_GlobalStatus->setText("Ожидание рисования буквы пользователем...");

        DrawingDialog dialog(this);

        // exec() запускает модальный режим и ждет, пока пользователь нажмет "Готово" (accept) или закроет окно
        if (dialog.exec() == QDialog::Accepted) {
            // Формируем путь для сохранения временного файла в текущей директории программы
            QString save_path = QDir::currentPath() + "/temp_drawn_letter.bmp";

            if (dialog.saveToFile(save_path)) {
                drawLetter(save_path);

                ui_->label_GlobalStatus->setText("Рисунок успешно сохранен в BMP и загружен для анализа.");
            } else {
                ui_->label_GlobalStatus->setText("Ошибка: Не удалось сохранить нарисованное изображение в файл BMP");
            }
        } else {
            ui_->label_GlobalStatus->setText("Рисование отменено");
        }
    }

    void MainWindow::on_btn_IdentifyLetter_clicked() noexcept {
        if (!image_.isNull()) {
            ui_->label_GlobalStatus->setText("Подготовка изображения к верификации");
            ui_->label_GlobalStatus->repaint();

            // Векторизация и нормализация в диапазон [0.0, 1.0]
            std::vector<float> data_vector;
            data_vector.reserve(DEFAULT_WIDTH_RES * DEFAULT_HEIGHT_RES);

            // Проходим по пикселям. В формате Grayscale8 значение цвета совпадает с индексом серого
            for (int y = 0; y < DEFAULT_HEIGHT_RES; ++y) {
                for (int x = 0; x < DEFAULT_WIDTH_RES; ++x) {
                    int pixel_val = qGray(image_.pixel(x, y));

                    // Нормализуем значение (0 -> 0.0, 255 -> 1.0)
                    float normalized = static_cast<float>(pixel_val) / 255.0f;
                    data_vector.push_back(normalized);
                }
            }

            ui_->label_GlobalStatus->setText("Перцептрон определяет букву на изображении...");
            ui_->label_GlobalStatus->repaint();
            auto res = controller_->verify(data_vector);

            ui_->label_IdentifyResult->setVisible(true);
            ui_->label_IdentifyResult->setText("Результат: " + QString(res));

            ui_->label_GlobalStatus->setText("Верификация завершена");
        }
        else {
            ui_->label_GlobalStatus->setText("Для начала загрузите готовое изображение или нарисуйте свое");
        }
    }

    void MainWindow::on_btn_SubNavigationButton_clicked(int index) noexcept {
        ui_->widget_TrainStack->setCurrentIndex(index);
    }

    void MainWindow::on_spin_CountEpochs_valueChanged() noexcept {
        count_epochs_ = ui_->spin_EpochsGroups->value();
        ui_->axisX_Training->setRange(0, count_epochs_ - 1);
    }

    void MainWindow::on_btn_LoadTrainSample_clicked() noexcept {
        QString fileName = QFileDialog::getOpenFileName(
            this, "Открыть тренировочную выборку", QString(),
            "CSV Files (*.csv);;All Files (*)");

        if (!fileName.isEmpty()) {
            qDebug() << "Выбран файл " << fileName;

            current_train_sample_ = fileName;

            ui_->label_NormalTrainSampleStatus->setVisible(true);
            ui_->label_ValidationTrainSampleStatus->setVisible(true);
            ui_->label_NormalTrainSampleStatus->setText("Загружен файл " + QFileInfo(fileName).fileName());
            ui_->label_ValidationTrainSampleStatus->setText("Загружен файл " + QFileInfo(fileName).fileName());
            ui_->label_GlobalStatus->setText("Выбрана тренировочная выборка");
        }
    }

    void MainWindow::on_btn_StartNormalTraining_clicked() noexcept {
        qDebug() << "Загружается файл с тренировочной выборкой: " << current_train_sample_;
        try {
            ui_->label_GlobalStatus->setText("Файл с тренировочной выборкой загружается в программу");
            ui_->label_GlobalStatus->repaint();
            controller_->open(current_train_sample_.toStdString());

            ui_->label_GlobalStatus->setText("Идет стандартное обучение перцептрона...");
            ui_->label_GlobalStatus->repaint();
            auto graphic_data = controller_->training(count_epochs_);

            ui_->label_GlobalStatus->setText("Строится график");
            ui_->label_GlobalStatus->repaint();
            drawTrainGraph(graphic_data);

            ui_->label_GlobalStatus->setText("Обучение завершено");
        }
        catch (const std::exception& e) {
            ui_->label_GlobalStatus->setText(e.what());
        }

        qDebug() << "Завершено";
    }

    void MainWindow::on_spin_CrossValidationK_valueChanged() noexcept {
        k_ = ui_->spin_KGroups->value();
    }

    void MainWindow::on_btn_StartCrossValidationTraining_clicked() noexcept {
        qDebug() << "Загружается файл с тренировочной выборкой: " << current_train_sample_;
        try {
            ui_->label_GlobalStatus->setText("Файл с тренировочной выборкой загружается в программу");
            ui_->label_GlobalStatus->repaint();
            controller_->open(current_train_sample_.toStdString());

            ui_->label_GlobalStatus->setText("Идет стандартное обучение с применением кросс-валидации...");
            ui_->label_GlobalStatus->repaint();
            auto res = controller_->crossValidation(k_);

            ui_->label_GlobalStatus->setText("Выводятся метрики");
            ui_->label_GlobalStatus->repaint();

            ui_->group_CrossValResults->setVisible(true);
            ui_->label_ResAccuracy->setText(QString("Average accuracy: %1").arg(res.accuracy()));
            ui_->label_ResPrecision->setText(QString("Precision: %1").arg(res.precision()));
            ui_->label_ResRecall->setText(QString("Recall: %1").arg(res.recall()));
            ui_->label_ResFMeasure->setText(QString("F-measure: %1").arg(res.recall()));
            ui_->label_ResTime->setText(QString("Time spent (sec): %1").arg(res.time() / 1000.0f));

            ui_->label_GlobalStatus->setText("Обучение завершено");
        }
        catch (const std::exception& e) {
            ui_->label_GlobalStatus->setText(e.what());
        }

        qDebug() << "Завершено";
    }

    void MainWindow::on_combo_PerceptronType_indexChanged(int index) noexcept {
        perc::PerceptronType type;

        if (index == 0) {
            type = perc::MATRIX_VIEW;

            qDebug() << "Выбрана матричная реализация перцептрона";
            ui_->label_GlobalStatus->setText("Выбрана матричная реализация перцептрона");
        }
        else {
            type = perc::GRAPH_VIEW;

            qDebug() << "Выбрана графовая реализация перцептрона";
            ui_->label_GlobalStatus->setText("Выбрана графовая реализация перцептрона");
        }

        controller_->switchImplementation(type);
    }

    void MainWindow::on_spin_CountHiddenLayers_valueChanged() noexcept {
        int count = ui_->spin_HiddenLayers->value();

        ui_->label_GlobalStatus->setText(QString("Изменено количество скрытых слоев: %1").arg(count));
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
            controller_->saveWeights(fileName.toStdString());
            ui_->label_GlobalStatus->setText("Веса перцептрона успешно сохранены!");
        }
    }

    void MainWindow::on_btn_LoadWeights_clicked() noexcept {
        QString fileName = QFileDialog::getOpenFileName(
            this, "Открыть файл с весами", QString(),
            "Weights File (*.weights);;All Files (*)");

        if (!fileName.isEmpty()) {
            qDebug() << "Загружен файл " << fileName;

            controller_->open(fileName.toStdString());
            ui_->label_GlobalStatus->setText("Веса перцептрона успешно загружены!");
        }
    }

}
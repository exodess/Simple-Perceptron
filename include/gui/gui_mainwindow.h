#ifndef PERCEPTRON_GUI_MAIN_WINDOW_H
#define PERCEPTRON_GUI_MAIN_WINDOW_H

#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QStackedWidget>
#include <QSlider>
#include <QGroupBox>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "data/data.h"

QT_BEGIN_NAMESPACE

class GUI_MainWindow {
public:
    QWidget *widget_CentralWidget; ///< Центральный виджет окна
    QVBoxLayout *layout_RootVertical; ///< Вертикальная корневая разметка
    QHBoxLayout *layout_RootHorizontal; ///< Горизонтальный корень

    // Панель навигации (левая колонка)
    QWidget *widget_NavigationPanel; ///< Контейнер кнопок
    QVBoxLayout *layout_NavigationPanel; ///< Layout панели навигации
    QButtonGroup *group_NavigationButtons; ///< Эксклюзивная группа кнопок
    QPushButton *btn_ExperimentPage; ///< Кнопка «Эксперимент»
    QPushButton *btn_TrainingPage; ///< Кнопка «Обучение»
    QPushButton *btn_SettingsPage; ///< Кнопка «Настройки»

    // Область содержимого (правая колонка)
    QStackedWidget *widget_ContentStack; ///< Переключаемые страницы

    QLabel *label_GlobalStatus; ///< Общая информационная линия в самом низу

    // =================================
    // Содержимое страницы "Эксперимент"
    // =================================
    QWidget *widget_ExperimentalPage;
    QVBoxLayout *layout_ExperimentalPage;

    // Область "Тестирование датасетом"
    QGroupBox *group_DatasetTesting;
    QVBoxLayout *layout_DatasetTesting;
    QPushButton *btn_LoadTestSample; ///< Кнопка "Загрузить тестовую выборку"
    QSlider *slider_TestSample; ///< Ползунок (скрыт до загрузки)
    QLabel *label_TestSampleStatus; ///< Надпись "Загружен файл <имя файла>"

    // Метрики в центре
    QWidget *widget_TestMetricsCenter;
    QVBoxLayout *layout_TestMetricsCenter;
    QLabel *label_TestAccuracy;
    QLabel *label_TestPrecision;
    QLabel *label_TestRecall;
    QLabel *label_TestFMeasure;
    QLabel *label_TestTime;

    QPushButton *btn_StartTesting; ///< Кнопка "Начать тестирование"

    // Область "Загрузить"
    QGroupBox *group_LoadSingle;
    QVBoxLayout *layout_LoadSingleRoot;
    QHBoxLayout *layout_LoadSingleTopRow; ///< Строка для верхних кнопок
    QPushButton *btn_LoadImage; ///< Кнопка "Загрузить готовую картинку"
    QPushButton *btn_DrawLetter; ///< Кнопка "Нарисовать букву"

    // Новая отдельная область для вывода BMP изображения
    QGroupBox *group_PreviewArea; ///< Область "Просмотр BMP"
    QVBoxLayout *layout_PreviewArea;
    QLabel *label_ImagePreview; ///< QLabel для отрисовки картинки

    QHBoxLayout *layout_LoadSingleBottomRow; ///< Строка для результатов и кнопки идентификации
    QLabel *label_IdentifyResult; ///< Надпись "Результат: <буква>"
    QPushButton *btn_Identify; ///< Кнопка "Идентифицировать"

    // ==============================
    // Содержимое страницы "Обучение"
    // ==============================
    QWidget *widget_TrainingPage;
    QVBoxLayout *layout_TrainingPage;

    // Внутренняя навигация
    QWidget *widget_TrainNav;
    QHBoxLayout *layout_TrainNav;
    QButtonGroup *group_TrainNav;
    QPushButton *btn_NormalTrain; ///< Кнопка "Обычное обучение"
    QPushButton *btn_CrossValTrain; ///< Кнопка "Обучение с кросс-валидацией"
    QStackedWidget *widget_TrainStack;

    // Подстраница "Обычное обучение"
    QWidget *widget_NormalTrainPage;
    QVBoxLayout *layout_NormalTrainPage;

    QPushButton *btn_LoadNormalTrainSample; ///< Кнопка "Загрузить выборку"
    QLabel *label_NormalTrainSampleStatus; ///< Надпись "Загружена выборка: <имя_файла>"
    QPushButton *btn_StartNormalTrain; ///< Кнопка "Начать обучение"

    // Компоненты постоянного графика обучения
    QChartView *chartView_Training;
    QChart *chart_Training;
    QLineSeries *series_Training;
    QValueAxis *axisX_Training;
    QValueAxis *axisY_Training;

    QWidget *widget_CrossValSettings;
    QFormLayout *layout_CrossValSettings;
    QLabel *label_EpochsGroups;
    QSpinBox *spin_KGroups; ///< Выбор количества эпох обучения
    QWidget *widget_GraphArea; ///< Контейнер для графика

    // Подстраница "Кросс-валидация"
    QWidget *widget_CrossValTrainPage;
    QVBoxLayout *layout_CrossValTrainPage;

    QWidget *widget_CountEpochsSettings;
    QFormLayout *layout_CountEpochsSettings;
    QLabel *label_KGroups;
    QSpinBox *spin_EpochsGroups; ///< Выбор количества групп k

    QPushButton *btn_LoadValidationTrainSample; ///< Кнопка "Загрузить выборку"
    QLabel *label_ValidationTrainSampleStatus; ///< Надпись "Загружена выборка: <имя_файла>"
    QPushButton *btn_StartCrossValTrain; ///< Кнопка "Начать обучение" (кросс-валидация)

    QGroupBox *group_CrossValResults; ///< Область "Результат"
    QVBoxLayout *layout_CrossValResults;
    QLabel *label_ResAccuracy;
    QLabel *label_ResPrecision;
    QLabel *label_ResRecall;
    QLabel *label_ResFMeasure;
    QLabel *label_ResTime;

    // ==============================
    // Содержимое страницы "Настройка"
    // ==============================
    QWidget *widget_LoadPage;
    QVBoxLayout *layout_LoadPage;

    // Область "Общие настройки"
    QGroupBox *group_GeneralSettings;
    QFormLayout *layout_GeneralSettings;
    QLabel *label_ImplMenu;
    QComboBox *combo_ImplMenu; ///< Меню выбора реализации перцептрона
    QLabel *label_HiddenLayers;
    QSpinBox *spin_HiddenLayers; ///< Переключатель скрытых слоев

    // Область "Настройка весов"
    QGroupBox *group_WeightsSettings;
    QVBoxLayout *layout_WeightsSettings;
    QHBoxLayout *layout_WeightsButtons;
    QPushButton *btn_SaveWeights; ///< Кнопка "Сохранить"
    QPushButton *btn_LoadWeights; ///< Кнопка "Загрузить"

    void setupUI() {
        widget_CentralWidget = new QWidget();

        // Создаем главный вертикальный контейнер для всего окна
        layout_RootVertical = new QVBoxLayout(widget_CentralWidget);
        layout_RootVertical->setContentsMargins(8, 8, 8, 8);
        layout_RootVertical->setSpacing(6); // Небольшой отступ между контентом и статус-баром

        // Создаем горизонтальный контейнер
        layout_RootHorizontal = new QHBoxLayout(widget_CentralWidget);
        layout_RootHorizontal->setContentsMargins(8, 8, 8, 8);
        layout_RootHorizontal->setSpacing(8);

        // Инициализируем навигационную панель
        setupNavigationPanel();

        // Настройка Content Area
        widget_ContentStack = new QStackedWidget;
        createExperimentalPage();
        createTrainingPage();
        createLoadPage();

        // Добавляем стек страниц в горизонтальный слой справа от меню
        layout_RootHorizontal->addWidget(widget_ContentStack, 1);

        // Добавляем весь этот горизонтальный блок в главный вертикальный слой с фактором растяжения 1
        layout_RootVertical->addLayout(layout_RootHorizontal, 1);

        // 4. Создаем единую независимую информационную линию снизу
        label_GlobalStatus = new QLabel("Программа готова к работе");

        QFont font_Status = label_GlobalStatus->font();
        font_Status.setPointSize(10);
        font_Status.setItalic(true);
        label_GlobalStatus->setFont(font_Status);

        // Добавляем легкую визуальную границу сверху строки, чтобы аккуратно отделить её
        label_GlobalStatus->setStyleSheet(
            "QLabel {"
            "  color: #555555;"
            "  border-top: 1px solid #ccc;"
            "  padding-top: 4px;"
            "  padding-left: 2px;"
            "}"
        );

        // Добавляем информационную линию в самый низ окна
        layout_RootVertical->addWidget(label_GlobalStatus);
    }

    void setupNavigationPanel() {
        widget_NavigationPanel = new QWidget();
        layout_NavigationPanel = new QVBoxLayout(widget_NavigationPanel);
        layout_NavigationPanel->setContentsMargins(0, 0, 0, 0);
        layout_NavigationPanel->setSpacing(0);

        group_NavigationButtons = new QButtonGroup();
        group_NavigationButtons->setExclusive(true);

        btn_ExperimentPage = createNavButton("Эксперимент");
        btn_TrainingPage = createNavButton("Обучение");
        btn_SettingsPage = createNavButton("Настройки");

        // Стили кнопок
        for (auto btn : {btn_ExperimentPage, btn_TrainingPage, btn_SettingsPage}) {
            auto font = btn->font();
            font.setPointSize(13);
            font.setItalic(true);
            btn->setFont(font);
        }

        // Индексы совпадают с порядком страниц в m_contentStack
        group_NavigationButtons->addButton(btn_ExperimentPage, 0);
        group_NavigationButtons->addButton(btn_TrainingPage, 1);
        group_NavigationButtons->addButton(btn_SettingsPage, 2);

        layout_NavigationPanel->addWidget(btn_ExperimentPage);
        layout_NavigationPanel->addWidget(btn_TrainingPage);
        layout_NavigationPanel->addWidget(btn_SettingsPage);

        widget_NavigationPanel->setFixedWidth(140);
        layout_RootHorizontal->addWidget(widget_NavigationPanel);

        // Активируем первую кнопку по умолчанию
        btn_ExperimentPage->setChecked(true);
    }

    void createExperimentalPage() {
        // Создание страницы "Эксперимент"
        widget_ExperimentalPage = new QWidget;
        layout_ExperimentalPage = new QVBoxLayout(widget_ExperimentalPage);

        layout_ExperimentalPage->setContentsMargins(16, 16, 16, 16);
        layout_ExperimentalPage->setSpacing(16);

        QFont font_Group;
        font_Group.setPointSize(12);
        font_Group.setBold(true);

        QFont font_Normal;
        font_Normal.setPointSize(11);

        // ================================================================
        // ============= ОБЛАСТЬ "Тестирование датасетом" =================
        // ================================================================
        group_DatasetTesting = new QGroupBox("Тестирование датасетом");
        group_DatasetTesting->setFont(font_Group);

        layout_DatasetTesting = new QVBoxLayout(group_DatasetTesting);
        layout_DatasetTesting->setSpacing(12);

        // Верхняя строка: Кнопка загрузки выборки и слайдер справа от нее
        auto *rowTopDataset = new QWidget;
        auto *layout_RowTopDataset = new QHBoxLayout(rowTopDataset);
        layout_RowTopDataset->setContentsMargins(0, 0, 0, 0);
        layout_RowTopDataset->setSpacing(12);

        btn_LoadTestSample = new QPushButton("Загрузить тестовую выборку");
        btn_LoadTestSample->setFont(font_Normal);
        layout_RowTopDataset->addWidget(btn_LoadTestSample);

        slider_TestSample = new QSlider(Qt::Horizontal);
        slider_TestSample->setRange(0, 100);
        slider_TestSample->setValue(100);
        slider_TestSample->setVisible(false); // Скрыт до момента клика по кнопке
        layout_RowTopDataset->addWidget(slider_TestSample);

        layout_RowTopDataset->addStretch(); // Сдвигает элементы строки влево
        layout_DatasetTesting->addWidget(rowTopDataset);

        // Статус загруженного файла (появляется ниже кнопки)
        label_TestSampleStatus = new QLabel("");
        label_TestSampleStatus->setVisible(false);
        layout_DatasetTesting->addWidget(label_TestSampleStatus);

        // Информационный столбик в центре области
        widget_TestMetricsCenter = new QWidget;
        layout_TestMetricsCenter = new QVBoxLayout(widget_TestMetricsCenter);
        layout_TestMetricsCenter->setContentsMargins(0, 0, 0, 0);
        layout_TestMetricsCenter->setSpacing(6);
        layout_TestMetricsCenter->setAlignment(Qt::AlignCenter); // Выравнивание по центру контейнера

        label_TestAccuracy = new QLabel("Average accuracy: -");
        label_TestPrecision = new QLabel("Precision: -");
        label_TestRecall = new QLabel("Recall: -");
        label_TestFMeasure = new QLabel("F-measure: -");
        label_TestTime = new QLabel("Time spent: -");

        for (auto lbl : {label_TestAccuracy, label_TestPrecision, label_TestRecall, label_TestFMeasure, label_TestTime}) {
            lbl->setFont(font_Normal);
            lbl->setAlignment(Qt::AlignCenter);
            layout_TestMetricsCenter->addWidget(lbl);
        }
        widget_TestMetricsCenter->setVisible(false);
        layout_DatasetTesting->addWidget(widget_TestMetricsCenter);

        // Нижняя строка: Кнопка "Начать тестирование" (внизу справа)
        auto *rowBottomDataset = new QWidget;
        auto *layout_RowBottomDataset = new QHBoxLayout(rowBottomDataset);
        layout_RowBottomDataset->setContentsMargins(0, 0, 0, 0);
        layout_RowBottomDataset->addStretch(); // Сдвигает кнопку вправо

        btn_StartTesting = new QPushButton("Начать тестирование");
        btn_StartTesting->setFixedWidth(200);
        btn_StartTesting->setStyleSheet(
            "QPushButton {"
            "  background-color: #4CAF50;"
            "  color: white;"
            "  border: none;"
            "  border-radius: 4px;"
            "  padding: 6px 12px;"
            "}"
            "QPushButton:hover  { background-color: #45A049; }"
            "QPushButton:pressed{ background-color: #388E3C; }"
        );
        btn_StartTesting->setFont(font_Normal);
        layout_RowBottomDataset->addWidget(btn_StartTesting);
        layout_DatasetTesting->addWidget(rowBottomDataset);

        layout_ExperimentalPage->addWidget(group_DatasetTesting);

        // ========================================================
        // ================= ОБЛАСТЬ "Загрузить" ==================
        // ========================================================
        group_LoadSingle = new QGroupBox("Загрузить");
        group_LoadSingle->setFont(font_Group);
        group_LoadSingle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        layout_LoadSingleRoot = new QVBoxLayout(group_LoadSingle);
        layout_LoadSingleRoot->setContentsMargins(16, 16, 16, 16);
        layout_LoadSingleRoot->setSpacing(12);

        // Верхний уровень: управляющие кнопки
        layout_LoadSingleTopRow = new QHBoxLayout();
        layout_LoadSingleTopRow->setContentsMargins(0, 0, 0, 0);
        layout_LoadSingleTopRow->setSpacing(12);

        btn_LoadImage = new QPushButton("Загрузить готовую картинку");
        btn_LoadImage->setFont(font_Normal);
        btn_LoadImage->setFixedWidth(260);

        btn_DrawLetter = new QPushButton("Нарисовать букву");
        btn_DrawLetter->setFont(font_Normal);
        btn_DrawLetter->setFixedWidth(180);

        layout_LoadSingleTopRow->addWidget(btn_LoadImage);
        layout_LoadSingleTopRow->addWidget(btn_DrawLetter);
        layout_LoadSingleTopRow->addStretch(); // Сдвигает кнопки к левому краю
        layout_LoadSingleRoot->addLayout(layout_LoadSingleTopRow);

        // Средний уровень: Отдельная центральная область просмотра BMP
        group_PreviewArea = new QGroupBox;
        // Применяем чуть менее жирный шрифт для внутреннего подзаголовка
        QFont font_SubGroup = font_Group;
        font_SubGroup.setBold(false);
        font_SubGroup.setItalic(true);
        group_PreviewArea->setFont(font_SubGroup);
        group_PreviewArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        layout_PreviewArea = new QVBoxLayout(group_PreviewArea);
        layout_PreviewArea->setContentsMargins(8, 12, 8, 8);

        label_ImagePreview = new QLabel();
        label_ImagePreview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label_ImagePreview->setMinimumSize(200, 200);
        // Белый фон внутри рамки области просмотра
        label_ImagePreview->setStyleSheet("background-color: #ffffff; border: 1px solid #ccc;");
        label_ImagePreview->setAlignment(Qt::AlignCenter);

        layout_PreviewArea->addWidget(label_ImagePreview);
        // Добавляем область в корень с фактором растяжения 1, чтобы она занимала все свободное место
        layout_LoadSingleRoot->addWidget(group_PreviewArea, 1);

        // Нижний уровень: Результат по центру и Кнопка справа
        layout_LoadSingleBottomRow = new QHBoxLayout();
        layout_LoadSingleBottomRow->setContentsMargins(0, 0, 0, 0);

        // Первый stretch отталкивает надпись результата к центру
        layout_LoadSingleBottomRow->addStretch(1);

        label_IdentifyResult = new QLabel("");
        QFont font_Result = font_Normal;
        font_Result.setBold(true);
        font_Result.setPointSize(13); // Сделаем результат чуть крупнее и заметнее
        label_IdentifyResult->setFont(font_Result);
        label_IdentifyResult->setAlignment(Qt::AlignCenter);
        label_IdentifyResult->setVisible(false); // Скрыта до момента клика "Идентифицировать"
        layout_LoadSingleBottomRow->addWidget(label_IdentifyResult);

        // Второй stretch балансирует надпись ровно посередине и прижимает кнопку в крайнее правое положение
        layout_LoadSingleBottomRow->addStretch(1);

        btn_Identify = new QPushButton("Идентифицировать");
        btn_Identify->setFont(font_Normal);
        btn_Identify->setFixedWidth(200);
        btn_Identify->setMinimumHeight(35);
        layout_LoadSingleBottomRow->addWidget(btn_Identify);

        layout_LoadSingleRoot->addLayout(layout_LoadSingleBottomRow);

        layout_ExperimentalPage->addWidget(group_LoadSingle);
        widget_ContentStack->addWidget(widget_ExperimentalPage);
    }

    void createTrainingPage() {
        widget_TrainingPage = new QWidget;
        layout_TrainingPage = new QVBoxLayout(widget_TrainingPage);
        layout_TrainingPage->setContentsMargins(16, 16, 16, 16);
        layout_TrainingPage->setSpacing(16);

        // Внутренняя навигация в верхней части
        widget_TrainNav = new QWidget;
        layout_TrainNav = new QHBoxLayout(widget_TrainNav);
        layout_TrainNav->setContentsMargins(0, 0, 0, 0);
        layout_TrainNav->setSpacing(12);

        group_TrainNav = new QButtonGroup(widget_TrainingPage);
        group_TrainNav->setExclusive(true);

        btn_NormalTrain = new QPushButton("Обычное обучение");
        btn_NormalTrain->setCheckable(true);
        btn_NormalTrain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        btn_NormalTrain->setMinimumHeight(45);

        btn_CrossValTrain = new QPushButton("Обучение с кросс-валидацией");
        btn_CrossValTrain->setCheckable(true);
        btn_CrossValTrain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        btn_CrossValTrain->setMinimumHeight(45);

        QFont font_TrainNav = btn_NormalTrain->font();
        font_TrainNav.setPointSize(12);
        btn_NormalTrain->setFont(font_TrainNav);
        btn_CrossValTrain->setFont(font_TrainNav);

        group_TrainNav->addButton(btn_NormalTrain, 0);
        group_TrainNav->addButton(btn_CrossValTrain, 1);

        layout_TrainNav->addWidget(btn_NormalTrain);
        layout_TrainNav->addWidget(btn_CrossValTrain);

        layout_TrainingPage->addWidget(widget_TrainNav);

        // Внутренний стек страницы
        widget_TrainStack = new QStackedWidget;

        // ===================================
        // === Страница "Обычное обучение" ===
        // ===================================
        widget_NormalTrainPage = new QWidget;
        layout_NormalTrainPage = new QVBoxLayout(widget_NormalTrainPage);
        layout_NormalTrainPage->setContentsMargins(0, 8, 0, 0);
        layout_NormalTrainPage->setSpacing(12);

        widget_CountEpochsSettings = new QWidget;
        layout_CountEpochsSettings = new QFormLayout(widget_CountEpochsSettings);
        layout_CountEpochsSettings->setContentsMargins(0, 0, 0, 0);

        label_EpochsGroups = new QLabel("Количество эпох:");
        label_EpochsGroups->setFont(font_TrainNav);
        spin_EpochsGroups = new QSpinBox;
        spin_EpochsGroups->setRange(MIN_COUNT_EPOCHS, MAX_COUNT_EPOCHS);
        spin_EpochsGroups->setSingleStep(1);
        spin_EpochsGroups->setValue(DEFAULT_COUNT_EPOCHS);
        spin_EpochsGroups->setFont(font_TrainNav);
        layout_CountEpochsSettings->addRow(label_EpochsGroups, spin_EpochsGroups);

        layout_NormalTrainPage->addWidget(widget_CountEpochsSettings);

        // Строка 1: Кнопка "Загрузить выборку" (слева)
        auto *rowLoadNormalBtnWidget = new QWidget;
        auto *layout_RowLoadNormalBtn = new QHBoxLayout(rowLoadNormalBtnWidget);
        layout_RowLoadNormalBtn->setContentsMargins(0, 0, 0, 0);

        btn_LoadNormalTrainSample = new QPushButton("Загрузить выборку");
        btn_LoadNormalTrainSample->setFixedWidth(180);
        btn_LoadNormalTrainSample->setFont(font_TrainNav);
        layout_RowLoadNormalBtn->addWidget(btn_LoadNormalTrainSample);
        layout_RowLoadNormalBtn->addStretch(); // Сдвигает кнопку влево
        layout_NormalTrainPage->addWidget(rowLoadNormalBtnWidget);

        // Строка 2: Надпись статуса загрузки (ниже кнопки загрузки, слева)
        auto *rowStatusWidget = new QWidget;
        auto *layout_RowStatus = new QHBoxLayout(rowStatusWidget);
        layout_RowStatus->setContentsMargins(0, 0, 0, 0);

        label_NormalTrainSampleStatus = new QLabel("");
        QFont font_Status = font_TrainNav;
        font_Status.setPointSize(11);
        font_Status.setItalic(true);
        label_NormalTrainSampleStatus->setFont(font_Status);
        label_NormalTrainSampleStatus->setVisible(false); // Скрыта до момента загрузки файла
        layout_RowStatus->addWidget(label_NormalTrainSampleStatus);
        layout_RowStatus->addStretch(); // Сдвигает надпись влево
        layout_NormalTrainPage->addWidget(rowStatusWidget);

        // Строка 3: Кнопка "Начать обучение" (справа, ширина по контенту)
        auto *rowStartBtnWidget = new QWidget;
        auto *layout_RowStartBtn = new QHBoxLayout(rowStartBtnWidget);
        layout_RowStartBtn->setContentsMargins(0, 0, 0, 0);
        layout_RowStartBtn->addStretch(); // Сдвигает кнопку вправо

        btn_StartNormalTrain = new QPushButton("Начать обучение");
        btn_StartNormalTrain->setMinimumHeight(40);
        btn_StartNormalTrain->setFont(font_TrainNav);

        // Ограничиваем ширину только размером текста внутри кнопки
        btn_StartNormalTrain->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        layout_RowStartBtn->addWidget(btn_StartNormalTrain);
        layout_NormalTrainPage->addWidget(rowStartBtnWidget);

        // Строка 4: Создание и инициализация пустого графика по умолчанию
        chart_Training = new QChart();
        series_Training = new QLineSeries();
        chart_Training->addSeries(series_Training);
        chart_Training->setTitle("Динамика обучения перцептрона");
        chart_Training->legend()->hide();

        // Настройка оси X (по умолчанию от 0 до 10 эпох)
        axisX_Training = new QValueAxis();
        axisX_Training->setTitleText("Эпохи");
        axisX_Training->setLabelFormat("%d");
        axisX_Training->setRange(0, DEFAULT_COUNT_EPOCHS - 1);
        chart_Training->addAxis(axisX_Training, Qt::AlignBottom);
        series_Training->attachAxis(axisX_Training);

        // Настройка оси Y (по умолчанию от 0.0 до 1.0)
        axisY_Training = new QValueAxis();
        axisY_Training->setTitleText("Значение контрольной ошибки");
        axisY_Training->setLabelFormat("%f");
        axisY_Training->setRange(0.0, 1.0);
        chart_Training->addAxis(axisY_Training, Qt::AlignLeft);
        series_Training->attachAxis(axisY_Training);

        // Создаем сам виджет отображения
        chartView_Training = new QChartView(chart_Training);
        chartView_Training->setRenderHint(QPainter::Antialiasing);
        chartView_Training->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // График сразу добавляется в компоновку страницы и виден по умолчанию
        layout_NormalTrainPage->addWidget(chartView_Training);
        widget_TrainStack->addWidget(widget_NormalTrainPage);

        // ================================================
        // ==== Страница "Обучение с кросс-валидацией" ====
        // ================================================
        widget_CrossValTrainPage = new QWidget;
        layout_CrossValTrainPage = new QVBoxLayout(widget_CrossValTrainPage);
        layout_CrossValTrainPage->setContentsMargins(0, 8, 0, 0);
        layout_CrossValTrainPage->setSpacing(12);

        widget_CrossValSettings = new QWidget;
        layout_CrossValSettings = new QFormLayout(widget_CrossValSettings);
        layout_CrossValSettings->setContentsMargins(0, 0, 0, 0);

        label_KGroups = new QLabel("Количество групп (k):");
        label_KGroups->setFont(font_TrainNav);
        spin_KGroups = new QSpinBox;
        spin_KGroups->setRange(MIN_K, MAX_K);
        spin_KGroups->setSingleStep(1);
        spin_KGroups->setValue(DEFAULT_K);
        spin_KGroups->setFont(font_TrainNav);
        layout_CrossValSettings->addRow(label_KGroups, spin_KGroups);

        layout_CrossValTrainPage->addWidget(widget_CrossValSettings);

        // Строка 1: Кнопка "Загрузить выборку" (слева)
        auto *rowLoadValidationBtnWidget = new QWidget;
        auto *layout_RowLoadValidationBtn = new QHBoxLayout(rowLoadValidationBtnWidget);
        layout_RowLoadValidationBtn->setContentsMargins(0, 0, 0, 0);

        btn_LoadValidationTrainSample = new QPushButton("Загрузить выборку");
        btn_LoadValidationTrainSample->setFixedWidth(180);
        btn_LoadValidationTrainSample->setFont(font_TrainNav);
        layout_RowLoadValidationBtn->addWidget(btn_LoadValidationTrainSample);
        layout_RowLoadValidationBtn->addStretch(); // Сдвигает кнопку влево
        layout_CrossValTrainPage->addWidget(rowLoadValidationBtnWidget);

        // Строка 2: Надпись статуса загрузки (ниже кнопки загрузки, слева)
        auto *rowValidationStatusWidget = new QWidget;
        auto *layout_RowValidationStatus = new QHBoxLayout(rowValidationStatusWidget);
        layout_RowValidationStatus->setContentsMargins(0, 0, 0, 0);

        label_ValidationTrainSampleStatus = new QLabel("");
        label_ValidationTrainSampleStatus->setFont(font_Status);
        label_ValidationTrainSampleStatus->setVisible(false); // Скрыта до момента загрузки файла
        layout_RowValidationStatus->addWidget(label_ValidationTrainSampleStatus);
        layout_RowValidationStatus->addStretch(); // Сдвигает надпись влево
        layout_CrossValTrainPage->addWidget(rowValidationStatusWidget);

        // Строка 3: Кнопка "Начать обучение" (справа, ширина по контенту)
        auto *rowStartValidationBtnWidget = new QWidget;
        auto *layout_RowStartValidationBtn = new QHBoxLayout(rowStartValidationBtnWidget);
        layout_RowStartValidationBtn->setContentsMargins(0, 0, 0, 0);
        layout_RowStartValidationBtn->addStretch(); // Сдвигает кнопку вправо

        btn_StartCrossValTrain = new QPushButton("Начать обучение");
        btn_StartCrossValTrain->setMinimumHeight(40);
        btn_StartCrossValTrain->setFont(font_TrainNav);
        btn_StartCrossValTrain->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        layout_RowStartValidationBtn->addWidget(btn_StartCrossValTrain);
        layout_CrossValTrainPage->addWidget(rowStartValidationBtnWidget);

        group_CrossValResults = new QGroupBox("Результат");
        QFont font_Group = group_CrossValResults->font();
        font_Group.setPointSize(12);
        font_Group.setBold(true);
        group_CrossValResults->setFont(font_Group);
        group_CrossValResults->setAlignment(Qt::AlignCenter);

        layout_CrossValResults = new QVBoxLayout(group_CrossValResults);
        layout_CrossValResults->setSpacing(8);

        QFont font_Res = font_TrainNav;
        font_Res.setPointSize(11);

        label_ResAccuracy = new QLabel("Average accuracy: -");
        label_ResPrecision = new QLabel("Precision: -");
        label_ResRecall = new QLabel("Recall: -");
        label_ResFMeasure = new QLabel("F-measure: -");
        label_ResTime = new QLabel("Общее затраченное время: -");

        for (auto lbl : {label_ResAccuracy, label_ResPrecision, label_ResRecall, label_ResFMeasure, label_ResTime}) {
            lbl->setFont(font_Res);
            layout_CrossValResults->addWidget(lbl);
        }

        layout_CrossValTrainPage->addWidget(group_CrossValResults);
        layout_CrossValTrainPage->addStretch(); // Сдвигаем всё наверх, чтобы элементы не разъезжались

        widget_TrainStack->addWidget(widget_CrossValTrainPage);

        layout_TrainingPage->addWidget(widget_TrainStack);

        // Активируем обычное обучение по умолчанию
        btn_NormalTrain->setChecked(true);

        widget_ContentStack->addWidget(widget_TrainingPage);
    }

    void createLoadPage() {
        widget_LoadPage = new QWidget;
        layout_LoadPage = new QVBoxLayout(widget_LoadPage);
        layout_LoadPage->setContentsMargins(16, 16, 16, 16);
        layout_LoadPage->setSpacing(16);

        // Общие настройки
        group_GeneralSettings = new QGroupBox("Общие настройки");
        QFont font_Group = group_GeneralSettings->font();
        font_Group.setPointSize(12);
        font_Group.setBold(true);
        group_GeneralSettings->setFont(font_Group);

        layout_GeneralSettings = new QFormLayout(group_GeneralSettings);
        layout_GeneralSettings->setLabelAlignment(Qt::AlignLeft);
        layout_GeneralSettings->setSpacing(12);

        QFont font_Normal = widget_LoadPage->font();
        font_Normal.setPointSize(11);

        // Выбор реализации перцептрона
        label_ImplMenu = new QLabel("Выбор реализации перцептрона:");
        label_ImplMenu->setFont(font_Normal);
        combo_ImplMenu = new QComboBox();
        combo_ImplMenu->addItem("Матричная");
        combo_ImplMenu->addItem("Графовая");
        combo_ImplMenu->setFont(font_Normal);
        layout_GeneralSettings->addRow(label_ImplMenu, combo_ImplMenu);

        // Переключение количества скрытых слоев
        label_HiddenLayers = new QLabel("Количество скрытых слоев:");
        label_HiddenLayers->setFont(font_Normal);
        spin_HiddenLayers = new QSpinBox();
        spin_HiddenLayers->setRange(MIN_HIDDEN, MAX_HIDDEN); // Ограничение от 2 до 5
        spin_HiddenLayers->setFont(font_Normal);

        layout_GeneralSettings->addRow(label_HiddenLayers, spin_HiddenLayers);
        layout_LoadPage->addWidget(group_GeneralSettings);

        // ===============
        // Настройка весов
        // ===============
        group_WeightsSettings = new QGroupBox("Настройка весов");
        group_WeightsSettings->setFont(font_Group);
        group_WeightsSettings->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        layout_WeightsSettings = new QVBoxLayout(group_WeightsSettings);
        layout_WeightsSettings->addStretch(); // Сдвигаем все последующие элементы (кнопки) вниз

        layout_WeightsButtons = new QHBoxLayout();
        layout_WeightsButtons->addStretch(); // Сдвигаем кнопки в правую сторону

        btn_SaveWeights = new QPushButton("Сохранить");
        btn_SaveWeights->setFixedWidth(120);
        btn_SaveWeights->setFont(font_Normal);

        btn_LoadWeights = new QPushButton("Загрузить");
        btn_LoadWeights->setFixedWidth(120);
        btn_LoadWeights->setFont(font_Normal);

        layout_WeightsButtons->addWidget(btn_SaveWeights);
        layout_WeightsButtons->addWidget(btn_LoadWeights);
        layout_WeightsSettings->addLayout(layout_WeightsButtons);

        layout_LoadPage->addWidget(group_WeightsSettings);

        // Добавляем страницу в основной стек
        widget_ContentStack->addWidget(widget_LoadPage);
    }

    QPushButton *createNavButton(const QString &text) {
        auto *btn = new QPushButton(text);

        btn->setCheckable(true);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn->setMinimumHeight(80);
        btn->setText(text);

        return btn;
    }
};

QT_END_NAMESPACE
#endif
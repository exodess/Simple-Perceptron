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

QT_BEGIN_NAMESPACE

class GUI_MainWindow {
public:
    QWidget *widget_CentralWidget; ///< Центральный виджет окна
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

    // =================================
    // Содержимое страницы "Эксперимент"
    // =================================
    QWidget *widget_ExperimentalPage;
    QLabel *label_ExperimentalPage;
    QLayout *layout_ExperimentalPage;

    QLabel *label_TitleLoad;
    QWidget *widget_RowData;
    QHBoxLayout *layout_Row;
    QPushButton *btn_LoadSample; ///< Кнопка "Загрузить выборку"
    QSlider *slider_Sample; ///< Ползунок (скрыт до загрузки)
    QPushButton *btn_StartSample; ///< Кнопка "Начать"

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
    QPushButton *btn_LoadTrainSample; ///< Кнопка "Загрузить выборку"
    QLabel *label_TrainSampleStatus; ///< Надпись "Загружена выборка: <имя_файла>"
    QPushButton *btn_StartNormalTrain; ///< Кнопка "Начать обучение"

    // Подстраница "Обычное обучение"
    QWidget *widget_NormalTrainPage;
    QVBoxLayout *layout_NormalTrainPage;
    QWidget *widget_GraphArea; ///< Контейнер для графика

    // Подстраница "Кросс-валидация"
    QWidget *widget_CrossValTrainPage;
    QVBoxLayout *layout_CrossValTrainPage;

    QWidget *widget_CrossValSettings;
    QFormLayout *layout_CrossValSettings;
    QLabel *label_KGroups;
    QSpinBox *spin_KGroups; ///< Выбор количества групп k

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

        layout_RootHorizontal = new QHBoxLayout(widget_CentralWidget);
        layout_RootHorizontal->setContentsMargins(8, 8, 8, 8);
        layout_RootHorizontal->setSpacing(8);

        setupNavigationPanel();

        // Настройка Content Area
        widget_ContentStack = new QStackedWidget;
        createExperimentalPage();
        createTrainingPage();
        createLoadPage();

        layout_RootHorizontal->addWidget(widget_ContentStack, 1);
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
        layout_ExperimentalPage->setSpacing(12);

        // Заголовок
        label_TitleLoad = new QLabel("Загрузить тестовую выборку");
        auto font_TitleLoad = label_TitleLoad->font();
        font_TitleLoad.setPointSize(13);
        font_TitleLoad.setBold(true);
        label_TitleLoad->setFont(font_TitleLoad);
        label_TitleLoad->setAlignment(Qt::AlignHCenter);
        layout_ExperimentalPage->addWidget(label_TitleLoad);

        auto rowWidget = new QWidget;
        auto rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(12);

        // Кнопка загрузить
        btn_LoadSample = new QPushButton("Загрузить");
        btn_LoadSample->setFixedWidth(100);
        QFont font_loadSample = btn_LoadSample->font();
        font_loadSample.setPointSize(13);
        btn_LoadSample->setFont(font_loadSample);
        rowLayout->addWidget(btn_LoadSample);

        slider_Sample = new QSlider(Qt::Horizontal);
        slider_Sample->setRange(0, 100);
        slider_Sample->setValue(0);
        slider_Sample->setVisible(false);   // скрыт до нажатия кнопки
        rowLayout->addWidget(slider_Sample);

        layout_ExperimentalPage->addWidget(rowWidget);

        // Кнопка «Начать» (внизу справа)
        auto *bottomRow = new QWidget;
        auto *bottomLayout = new QHBoxLayout(bottomRow);
        bottomLayout->setContentsMargins(0, 0, 0, 0);
        bottomLayout->addStretch(); // прижимаем кнопку вправо

        btn_StartSample = new QPushButton("Начать");
        btn_StartSample->setFixedWidth(150);
        btn_StartSample->setStyleSheet(
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
        QFont font_StartSample = btn_StartSample->font();
        font_StartSample.setPointSize(13);
        btn_StartSample->setFont(font_StartSample);
        bottomLayout->addWidget(btn_StartSample);

        layout_ExperimentalPage->addWidget(bottomRow);
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

        // Строка 1: Кнопка "Загрузить выборку" (слева)
        auto *rowLoadBtnWidget = new QWidget;
        auto *layout_RowLoadBtn = new QHBoxLayout(rowLoadBtnWidget);
        layout_RowLoadBtn->setContentsMargins(0, 0, 0, 0);

        btn_LoadTrainSample = new QPushButton("Загрузить выборку");
        btn_LoadTrainSample->setFixedWidth(180);
        btn_LoadTrainSample->setFont(font_TrainNav);
        layout_RowLoadBtn->addWidget(btn_LoadTrainSample);
        layout_RowLoadBtn->addStretch(); // Сдвигает кнопку влево
        layout_NormalTrainPage->addWidget(rowLoadBtnWidget);

        // Строка 2: Надпись статуса загрузки (ниже кнопки загрузки, слева)
        auto *rowStatusWidget = new QWidget;
        auto *layout_RowStatus = new QHBoxLayout(rowStatusWidget);
        layout_RowStatus->setContentsMargins(0, 0, 0, 0);

        label_TrainSampleStatus = new QLabel("");
        QFont font_Status = font_TrainNav;
        font_Status.setPointSize(11);
        font_Status.setItalic(true);
        label_TrainSampleStatus->setFont(font_Status);
        label_TrainSampleStatus->setVisible(false); // Скрыта до момента загрузки файла
        layout_RowStatus->addWidget(label_TrainSampleStatus);
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

        // Контейнер (заглушка) под график, который будет растягиваться
        widget_GraphArea = new QWidget;
        widget_GraphArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        widget_GraphArea->setStyleSheet("background-color: #f5f5f5; border: 1px solid #ccc;");

        // Временный текст внутри области графика (можно убрать при интеграции реального графика)
        QVBoxLayout *layout_Graph = new QVBoxLayout(widget_GraphArea);
        QLabel *label_GraphPlaceholder = new QLabel("Для построения графика начните обучение", widget_GraphArea);
        label_GraphPlaceholder->setAlignment(Qt::AlignCenter);
        layout_Graph->addWidget(label_GraphPlaceholder);

        layout_NormalTrainPage->addWidget(widget_GraphArea);
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
        spin_KGroups->setRange(10, 120);
        spin_KGroups->setSingleStep(1);
        spin_KGroups->setFont(font_TrainNav);
        layout_CrossValSettings->addRow(label_KGroups, spin_KGroups);

        layout_CrossValTrainPage->addWidget(widget_CrossValSettings);

        btn_StartCrossValTrain = new QPushButton("Начать обучение");
        btn_StartCrossValTrain->setMinimumHeight(40);
        btn_StartCrossValTrain->setFont(font_TrainNav);
        layout_CrossValTrainPage->addWidget(btn_StartCrossValTrain);

        group_CrossValResults = new QGroupBox("Результат");
        QFont font_Group = group_CrossValResults->font();
        font_Group.setPointSize(12);
        font_Group.setBold(true);
        group_CrossValResults->setFont(font_Group);

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
        spin_HiddenLayers->setRange(2, 5); // Ограничение от 2 до 5
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
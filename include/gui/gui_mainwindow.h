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
    QLabel *label_TrainingPage;
    QLayout *layout_TrainingPage;

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
        // Создание страницы "Тренировка"
        widget_TrainingPage = new QWidget;
        label_TrainingPage = new QLabel("Обучение", widget_TrainingPage);
        layout_TrainingPage = new QVBoxLayout(new QLabel("Обучение", widget_TrainingPage));

        label_TrainingPage->setAlignment(Qt::AlignCenter);
        layout_TrainingPage->addWidget(label_TrainingPage);

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
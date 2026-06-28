#ifndef PERCEPTRON_GUI_MAIN_WINDOW_H
#define PERCEPTRON_GUI_MAIN_WINDOW_H

#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QStackedWidget>
#include <QSlider>

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
    QPushButton *btn_LoadPage; ///< Кнопка «Загрузить»

    // Содержимое страницы "Обучение"
    // Содержимое страницы "Загрузить"

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
    // Содержимое страницы "Загрузка"
    // ==============================
    QWidget *widget_LoadPage;
    QLabel *label_LoadPage;
    QLayout *layout_LoadPage;

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
        btn_LoadPage = createNavButton("Загрузить");

        // Стили кнопок
        for (auto btn : {btn_ExperimentPage, btn_TrainingPage, btn_LoadPage}) {
            auto font = btn->font();
            font.setPointSize(13);
            font.setItalic(true);
            btn->setFont(font);
        }

        // Индексы совпадают с порядком страниц в m_contentStack
        group_NavigationButtons->addButton(btn_ExperimentPage, 0);
        group_NavigationButtons->addButton(btn_TrainingPage, 1);
        group_NavigationButtons->addButton(btn_LoadPage, 2);

        layout_NavigationPanel->addWidget(btn_ExperimentPage);
        layout_NavigationPanel->addWidget(btn_TrainingPage);
        layout_NavigationPanel->addWidget(btn_LoadPage);

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
        // Создание страницы "Тренировка"
        widget_LoadPage = new QWidget;
        label_LoadPage = new QLabel("Загрузить", widget_LoadPage);
        layout_LoadPage = new QVBoxLayout(new QLabel("Загрузить", widget_LoadPage));

        label_LoadPage->setAlignment(Qt::AlignHCenter);
        layout_LoadPage->addWidget(label_LoadPage);

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
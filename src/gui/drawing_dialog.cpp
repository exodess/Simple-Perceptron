#include "gui/drawing_dialog.h"

namespace gui {

    DrawingDialog::DrawingDialog(QWidget *parent) : QDialog(parent) {
        setWindowTitle("Окно рисования символа");
        setFixedSize(430, 490);
        setModal(true); // Блокирует главное окно, пока идет рисование

        auto *main_layout = new QVBoxLayout(this);
        main_layout->setContentsMargins(15, 15, 15, 15);

        // Добавляем наш холст по центру окна
        canvas_ = new DrawingCanvas(this);
        canvas_->setFixedSize(400, 400);
        canvas_->setStyleSheet("border: 1px solid #555;");
        main_layout->addWidget(canvas_, 0, Qt::AlignCenter);

        // Нижняя панель кнопок
        auto *btn_layout = new QHBoxLayout();
        auto *btn_clear = new QPushButton("Очистить", this);
        auto *btn_ready = new QPushButton("Готово", this);

        btn_clear->setMinimumHeight(30);
        btn_ready->setMinimumHeight(30);

        btn_layout->addWidget(btn_clear);
        btn_layout->addWidget(btn_ready);
        main_layout->addLayout(btn_layout);

        // Связываем логику кнопок
        connect(btn_clear, &QPushButton::clicked, canvas_, &DrawingCanvas::clear);
        connect(btn_ready, &QPushButton::clicked, this, &QDialog::accept);
    }

    bool DrawingDialog::saveToFile(const QString &file_path) const {
        return canvas_->saveBmp(file_path);
    }

}
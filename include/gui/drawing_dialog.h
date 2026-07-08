#ifndef DRAWING_DIALOG_H
#define DRAWING_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QPen>
#include <QHBoxLayout>
#include <QPushButton>

namespace gui {
    /**
     * @class DrawingCanvas
     * @brief Класс холста для рисования мышью
     */
    class DrawingCanvas : public QWidget {
    public:
        explicit DrawingCanvas(QWidget *parent = nullptr);

        void clear();
        bool saveBmp(const QString &file_path) const;

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    private:
        QImage canvas_;
        bool is_drawing_;
        QPoint last_point_;
    };

    /**
     * @class DrawingDialog
     * @brief Класс диалогового окна, объединяющий холст и кнопки управления
     */
    class DrawingDialog : public QDialog {
    public:
        explicit DrawingDialog(QWidget *parent = nullptr);

        bool saveToFile(const QString &file_path) const;

    private:
        DrawingCanvas *canvas_;
    };

}

#endif
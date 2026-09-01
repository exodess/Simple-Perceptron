#include "gui/drawing_dialog.h"

namespace gui {

    DrawingCanvas::DrawingCanvas(QWidget *parent) : QWidget(parent), is_drawing_(false) {
        setAttribute(Qt::WA_StaticContents);

        // Создаем холст 400x400. Использование Grayscale8 обеспечивает
        // качественное сглаживание (антиалиасинг) краев кисти, как в EMNIST.
        canvas_ = QImage(400, 400, QImage::Format_Grayscale8);
        canvas_.fill(Qt::white); // Изначально белый фон
    }

    void DrawingCanvas::clear() {
        canvas_.fill(Qt::white);
        update();
    }

    bool DrawingCanvas::saveBmp(const QString &file_path) const {
        return canvas_.save(file_path, "BMP");
    }

    void DrawingCanvas::paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.drawImage(0, 0, canvas_);
    }

    void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            last_point_ = event->pos();
            is_drawing_ = true;
        }
    }

    void DrawingCanvas::mouseMoveEvent(QMouseEvent *event) {
        if ((event->buttons() & Qt::LeftButton) && is_drawing_) {
            QPainter painter(&canvas_);

            // Настройка кисти: черный цвет, толщина 24 пикселя, скругленные края
            QPen pen(Qt::black, 24, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
            painter.setPen(pen);
            painter.setRenderHint(QPainter::Antialiasing); // Плавные линии

            painter.drawLine(last_point_, event->pos());
            last_point_ = event->pos();
            update();
        }
    }

    void DrawingCanvas::mouseReleaseEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            is_drawing_ = false;
        }
    }

}
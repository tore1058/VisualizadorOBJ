#include "canvaswidget.h"
#include "transformacao2d.h"
#include "objetoobj.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent) {
    setMinimumSize(600, 400);
}

void CanvasWidget::setDisplayFile(QList<ObjetoGrafico*> *lista) {
    displayFile = lista;
}

void CanvasWidget::setSelectedIndex(int index) {
    selectedIndex = index;
}

void CanvasWidget::setCamera(Camera *cam) {
    camera = cam;
}

void CanvasWidget::setControlMode(bool cam) {
    controlCamera = cam;
}

void CanvasWidget::paintEvent(QPaintEvent *) {
    if (!displayFile) return;

    QPainter painter(this);

    Transformacao2D tx;
    tx.window = QRectF(-1, -1, 2, 2);  // SCN: de -1 a +1
    tx.viewport = rect();             // viewport: área visível


    QPointF topLeft = tx.windowToViewport(QPointF(tx.window.left(), tx.window.top()));
    QPointF bottomRight = tx.windowToViewport(QPointF(tx.window.right(), tx.window.bottom()));
    QRectF clipRect(topLeft, bottomRight);
    clipRect = clipRect.normalized();  // garante que o retângulo seja desenhado corretamente

    QPen penClip(Qt::gray);
    penClip.setStyle(Qt::DashLine);
    penClip.setWidth(2);
    painter.setPen(penClip);

    painter.drawRect(clipRect);


    for (ObjetoGrafico* obj : *displayFile) {
        auto obj3d = dynamic_cast<ObjetoObj*>(obj);
        if (obj3d) {
            obj3d->setCamera(camera);
        }

        auto original = obj->getLinhasClipadas(
            [](QPointF p) { return p; },
            QRectF(-99999, -99999, 1e6, 1e6),
            [&](QPointF p) { return tx.windowToViewport(obj3d ? p : tx.worldToWindow(p)); }
            );

        QPen penOriginal(Qt::red);
        penOriginal.setStyle(Qt::DotLine);
        penOriginal.setWidth(1);
        painter.setPen(penOriginal);

        for (const QLineF &l : original)
            painter.drawLine(l);

        auto clipado = obj->getLinhasClipadas(
            [&](QPointF p) { return obj3d ? p : tx.worldToWindow(p); },
            tx.window,
            [&](QPointF p) { return tx.windowToViewport(p); }
            );

        QPen penClipado(Qt::black);
        penClipado.setWidth(2);
        painter.setPen(penClipado);

        for (const QLineF &l : clipado)
            painter.drawLine(l);
    }
}

void CanvasWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        lastPos = event->pos();
    }
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!dragging)
        return;

    QPoint delta = event->pos() - lastPos;

    if (controlCamera && camera) {
        camera->rotate(delta.y(), delta.x());
        lastPos = event->pos();
        update();
        return;
    }

    if (displayFile && selectedIndex >= 0 && selectedIndex < displayFile->size()) {
        auto obj3d = dynamic_cast<ObjetoObj*>((*displayFile)[selectedIndex]);
        if (obj3d) {
            obj3d->rotacionar3D(delta.y(), delta.x());
            lastPos = event->pos();
            update();
        }
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        dragging = false;
}

void CanvasWidget::wheelEvent(QWheelEvent *event) {
    if (controlCamera && camera) {
        int delta = event->angleDelta().y();
        double dist = delta > 0 ? -0.5 : 0.5;
        QVector3D dir = (camera->getTarget() - camera->getPosition()).normalized();
        camera->setPosition(camera->getPosition() + dir * dist);
        camera->setTarget(camera->getTarget() + dir * dist);
        update();
        return;
    }

    if (!displayFile || selectedIndex < 0 || selectedIndex >= displayFile->size())
        return;

    auto obj3d = dynamic_cast<ObjetoObj*>((*displayFile)[selectedIndex]);
    if (obj3d) {
        int delta = event->angleDelta().y();
        double fator = delta > 0 ? 1.1 : 0.9;
        obj3d->escalar3D(fator);
        update();
    }
}


#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QWidget>
#include <QList>
#include "objeto.h"
#include <QWheelEvent>
#include "camera.h"

class CanvasWidget : public QWidget {
    Q_OBJECT

public:
    explicit CanvasWidget(QWidget *parent = nullptr);
    void setDisplayFile(QList<ObjetoGrafico*> *lista);
    void setSelectedIndex(int index);
    void setCamera(Camera *cam);
    void setControlMode(bool cam);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QList<ObjetoGrafico*> *displayFile = nullptr;
    int selectedIndex = -1;
    bool dragging = false;
    QPoint lastPos;
    Camera *camera = nullptr;
    bool controlCamera = false;
};

#endif // CANVASWIDGET_H

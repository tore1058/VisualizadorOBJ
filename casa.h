#ifndef CASA_H
#define CASA_H

#include "objeto.h"
#include <QPoint>
#include <QVector>
#include <QLineF>

class Casa : public ObjetoGrafico {
    QVector<QPoint> pontosBase;
    QVector<QPoint> pontosTelhado;

public:
    Casa(const QPoint &topLeft, int largura, int altura);
    void desenhar(QPainter &painter) override;
    void transladar(int dx, int dy) override;
    void escalar(double sx, double sy) override;
    void rotacionar(double anguloGraus, int cx, int cy) override;

    QVector<QLineF> getLinhasClipadas(
        std::function<QPointF(QPointF)> worldToWindow,
        const QRectF &clippingWindow,
        std::function<QPointF(QPointF)> windowToViewport) const override;
};

#endif // CASA_H

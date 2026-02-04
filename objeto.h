#ifndef OBJETO_H
#define OBJETO_H

#include <QPainter>

class ObjetoGrafico {
public:
    virtual void desenhar(QPainter &painter) = 0;
    virtual void transladar(int dx, int dy) = 0;
    virtual void escalar(double sx, double sy) = 0;
    virtual void rotacionar(double anguloGraus, int cx, int cy) = 0;
    virtual QVector<QLineF> getLinhasClipadas(
        std::function<QPointF(QPointF)> worldToWindow,
        const QRectF &clippingWindow,
        std::function<QPointF(QPointF)> windowToViewport) const = 0;

    virtual ~ObjetoGrafico() {}
};

#endif // OBJETO_H

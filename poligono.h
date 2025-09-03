#ifndef POLIGONO_H
#define POLIGONO_H

#include "objeto.h"
#include <QList>
#include <QPoint>

class Poligono : public ObjetoGrafico {
    QList<QPoint> pontos;
public:
    Poligono(const QList<QPoint> &pts);
    void desenhar(QPainter &painter) override;
    void transladar(int dx, int dy) override;
    void escalar(double sx, double sy) override;
    void rotacionar(double anguloGraus, int cx, int cy) override;
    QVector<QLineF> getLinhasClipadas(
        std::function<QPointF(QPointF)> worldToWindow,
        const QRectF &clippingWindow,
        std::function<QPointF(QPointF)> windowToViewport) const override;

};

#endif // POLIGONO_H

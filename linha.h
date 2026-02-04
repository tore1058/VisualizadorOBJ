#ifndef LINHA_H
#define LINHA_H

#include "objeto.h"
#include <QPoint>

class Linha : public ObjetoGrafico {
    QPoint p1, p2;
public:
    Linha(const QPoint &a, const QPoint &b);
    void desenhar(QPainter &painter) override;
    void transladar(int dx, int dy) override;
    void escalar(double sx, double sy) override;
    void rotacionar(double anguloGraus, int cx, int cy) override;
    QVector<QLineF> getLinhasClipadas(
        std::function<QPointF(QPointF)> worldToWindow,
        const QRectF &clippingWindow,
        std::function<QPointF(QPointF)> windowToViewport) const override;

};

#endif // LINHA_H

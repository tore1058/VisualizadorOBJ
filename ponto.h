#ifndef PONTO_H
#define PONTO_H

#include "objeto.h"
#include <QPoint>

class Ponto : public ObjetoGrafico {
    QPoint posicao;
public:
    Ponto(const QPoint &p);
    void desenhar(QPainter &painter) override;
    void transladar(int dx, int dy) override;
    void escalar(double sx, double sy) override;
    void rotacionar(double anguloGraus, int cx, int cy) override;
    QVector<QLineF> getLinhasClipadas(
        std::function<QPointF(QPointF)> worldToWindow,
        const QRectF &clippingWindow,
        std::function<QPointF(QPointF)> windowToViewport) const override;

};

#endif // PONTO_H

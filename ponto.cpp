#include "ponto.h"
#include "matriz.h"
#include <QtMath>
#include <cmath>

Ponto::Ponto(const QPoint &p) : posicao(p) {}

void Ponto::desenhar(QPainter &painter) {
    painter.drawPoint(posicao);
}

void Ponto::transladar(int dx, int dy) {
    Matriz T = Matriz::translacao(dx, -dy);
    posicao = T.aplicar(posicao).toPoint();
}

void Ponto::escalar(double sx, double sy) {
    // Escalando em torno do próprio ponto para manter a posição
    Matriz T = Matriz::translacao(-posicao.x(), -posicao.y());
    Matriz S = Matriz::escala(sx, sy);
    Matriz T_inv = Matriz::translacao(posicao.x(), posicao.y());
    Matriz transformacao = T_inv * S * T;
    posicao = transformacao.aplicar(posicao).toPoint();
}


void Ponto::rotacionar(double anguloGraus, int cx, int cy) {
    double rad = qDegreesToRadians(anguloGraus);
    int x = posicao.x() - cx;
    int y = posicao.y() - cy;
    int novoX = cx + x * cos(rad) - y * sin(rad);
    int novoY = cy + x * sin(rad) + y * cos(rad);
    posicao = QPoint(novoX, novoY);
}

QVector<QLineF> Ponto::getLinhasClipadas(
    std::function<QPointF(QPointF)> worldToWindow,
    const QRectF &clippingWindow,
    std::function<QPointF(QPointF)> windowToViewport) const
{
    QVector<QLineF> resultado;
    QPointF w = worldToWindow(posicao);

    if (clippingWindow.contains(w)) {
        QPointF v = windowToViewport(w);
        resultado.append(QLineF(v, v));  // ponto como linha degenerada
    }

    return resultado;
}


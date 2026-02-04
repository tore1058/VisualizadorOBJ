#include "linha.h"
#include "matriz.h"
Linha::Linha(const QPoint &a, const QPoint &b) : p1(a), p2(b) {}

void Linha::desenhar(QPainter &painter) {
    painter.drawLine(p1, p2);
}

void Linha::transladar(int dx, int dy) {
    Matriz T = Matriz::translacao(dx, -dy);
    p1 = T.aplicar(p1).toPoint();
    p2 = T.aplicar(p2).toPoint();
}

void Linha::escalar(double sx, double sy) {
    QPoint centro = (p1 + p2) / 2;
    Matriz T = Matriz::translacao(-centro.x(), -centro.y());
    Matriz S = Matriz::escala(sx, sy);
    Matriz T_inv = Matriz::translacao(centro.x(), centro.y());

    Matriz transformacao = T_inv * S * T;
    p1 = transformacao.aplicar(p1).toPoint();
    p2 = transformacao.aplicar(p2).toPoint();
}

void Linha::rotacionar(double anguloGraus, int cx, int cy) {
    Matriz T = Matriz::translacao(-cx, -cy);
    Matriz R = Matriz::rotacao(anguloGraus);
    Matriz T_inv = Matriz::translacao(cx, cy);

    Matriz transformacao = T_inv * R * T;
    p1 = transformacao.aplicar(p1).toPoint();
    p2 = transformacao.aplicar(p2).toPoint();
}

static int computeOutCode(QPointF p, const QRectF &clip) {
    int code = 0;
    if (p.x() < clip.left())   code |= 1;
    if (p.x() > clip.right())  code |= 2;
    if (p.y() < clip.top())    code |= 4;
    if (p.y() > clip.bottom()) code |= 8;
    return code;
}

QVector<QLineF> Linha::getLinhasClipadas(
    std::function<QPointF(QPointF)> worldToWindow,
    const QRectF &clippingWindow,
    std::function<QPointF(QPointF)> windowToViewport) const
{
    QVector<QLineF> resultado;

    QPointF w0 = worldToWindow(p1);
    QPointF w1 = worldToWindow(p2);

    int outcode0 = computeOutCode(w0, clippingWindow);
    int outcode1 = computeOutCode(w1, clippingWindow);

    QPointF p0 = w0;
    QPointF p1_ = w1;

    while (true) {
        if (!(outcode0 | outcode1)) {
            // Totalmente dentro
            QPointF v0 = windowToViewport(p0);
            QPointF v1 = windowToViewport(p1_);
            resultado.append(QLineF(v0, v1));
            break;
        } else if (outcode0 & outcode1) {
            // Totalmente fora
            break;
        } else {
            // Clipping parcial
            double x, y;
            int outcodeOut = outcode0 ? outcode0 : outcode1;

            double xmin = clippingWindow.left(), xmax = clippingWindow.right();
            double ymin = clippingWindow.top(), ymax = clippingWindow.bottom();
            double x0 = p0.x(), y0 = p0.y();
            double x1 = p1_.x(), y1 = p1_.y();

            if (outcodeOut & 8) { // bottom
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if (outcodeOut & 4) { // top
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if (outcodeOut & 2) { // right
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else { // left
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            if (outcodeOut == outcode0) {
                p0 = QPointF(x, y);
                outcode0 = computeOutCode(p0, clippingWindow);
            } else {
                p1_ = QPointF(x, y);
                outcode1 = computeOutCode(p1_, clippingWindow);
            }
        }
    }

    return resultado;
}





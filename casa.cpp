#include "casa.h"
#include "matriz.h"

static int computeOutCode(QPointF p, const QRectF &clip) {
    int code = 0;
    if (p.x() < clip.left())   code |= 1;  // LEFT
    if (p.x() > clip.right())  code |= 2;  // RIGHT
    if (p.y() < clip.top())    code |= 4;  // TOP
    if (p.y() > clip.bottom()) code |= 8;  // BOTTOM
    return code;
}

Casa::Casa(const QPoint &topLeft, int largura, int altura) {
    int w = largura;
    int h = altura;
    int meio = topLeft.x() + w / 2;

    // Base retangular
    pontosBase = {
        topLeft,
        QPoint(topLeft.x() + w, topLeft.y()),
        QPoint(topLeft.x() + w, topLeft.y() + h),
        QPoint(topLeft.x(),     topLeft.y() + h)
    };

    // Telhado triangular
    pontosTelhado = {
        QPoint(topLeft.x(), topLeft.y()),
        QPoint(topLeft.x() + w, topLeft.y()),
        QPoint(meio, topLeft.y() - h / 2)
    };
}

void Casa::desenhar(QPainter &painter) {
    for (int i = 0; i < pontosBase.size(); ++i)
        painter.drawLine(pontosBase[i], pontosBase[(i + 1) % pontosBase.size()]);
    for (int i = 0; i < pontosTelhado.size(); ++i)
        painter.drawLine(pontosTelhado[i], pontosTelhado[(i + 1) % pontosTelhado.size()]);
}

void Casa::transladar(int dx, int dy) {
    Matriz T = Matriz::translacao(dx, -dy);
    for (QPoint &p : pontosBase)
        p = T.aplicar(p).toPoint();
    for (QPoint &p : pontosTelhado)
        p = T.aplicar(p).toPoint();
}

void Casa::escalar(double sx, double sy) {
    QPoint centro(0, 0);
    for (const QPoint &p : pontosBase)
        centro += p;
    for (const QPoint &p : pontosTelhado)
        centro += p;
    centro /= (pontosBase.size() + pontosTelhado.size());

    Matriz T = Matriz::translacao(-centro.x(), -centro.y());
    Matriz S = Matriz::escala(sx, sy);
    Matriz T_inv = Matriz::translacao(centro.x(), centro.y());
    Matriz transformacao = T_inv * S * T;

    for (QPoint &p : pontosBase)
        p = transformacao.aplicar(p).toPoint();
    for (QPoint &p : pontosTelhado)
        p = transformacao.aplicar(p).toPoint();
}

void Casa::rotacionar(double anguloGraus, int cx, int cy) {
    Matriz T = Matriz::translacao(-cx, -cy);
    Matriz R = Matriz::rotacao(anguloGraus);
    Matriz T_inv = Matriz::translacao(cx, cy);
    Matriz transformacao = T_inv * R * T;

    for (QPoint &p : pontosBase)
        p = transformacao.aplicar(p).toPoint();
    for (QPoint &p : pontosTelhado)
        p = transformacao.aplicar(p).toPoint();
}

QVector<QLineF> Casa::getLinhasClipadas(
    std::function<QPointF(QPointF)> worldToWindow,
    const QRectF &clippingWindow,
    std::function<QPointF(QPointF)> windowToViewport) const
{
    QVector<QLineF> resultado;
    auto addSegmentos = [&](const QVector<QPoint> &pontos) {
        for (int i = 0; i < pontos.size(); ++i) {
            QPointF a = worldToWindow(pontos[i]);
            QPointF b = worldToWindow(pontos[(i + 1) % pontos.size()]);
            // usa Cohen-Sutherland igual Linha
            int codeA = computeOutCode(a, clippingWindow);
            int codeB = computeOutCode(b, clippingWindow);

            while (true) {
                if (!(codeA | codeB)) {
                    // totalmente dentro
                    resultado.append(QLineF(
                        windowToViewport(a), windowToViewport(b)
                        ));
                    break;
                } else if (codeA & codeB) {
                    // totalmente fora
                    break;
                } else {
                    double x, y;
                    int outcodeOut = codeA ? codeA : codeB;
                    double xmin = clippingWindow.left(), xmax = clippingWindow.right();
                    double ymin = clippingWindow.top(), ymax = clippingWindow.bottom();
                    double x0 = a.x(), y0 = a.y(), x1 = b.x(), y1 = b.y();

                    if (outcodeOut & 8) { x = x0 + (x1 - x0)*(ymax - y0)/(y1 - y0); y = ymax; }
                    else if (outcodeOut & 4) { x = x0 + (x1 - x0)*(ymin - y0)/(y1 - y0); y = ymin; }
                    else if (outcodeOut & 2) { y = y0 + (y1 - y0)*(xmax - x0)/(x1 - x0); x = xmax; }
                    else { y = y0 + (y1 - y0)*(xmin - x0)/(x1 - x0); x = xmin; }

                    if (outcodeOut == codeA) {
                        a = QPointF(x, y);
                        codeA = computeOutCode(a, clippingWindow);
                    } else {
                        b = QPointF(x, y);
                        codeB = computeOutCode(b, clippingWindow);
                    }
                }
            }
        }
    };

    addSegmentos(pontosBase);
    addSegmentos(pontosTelhado);
    return resultado;
}

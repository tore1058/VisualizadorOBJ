#include "poligono.h"
#include "matriz.h"

Poligono::Poligono(const QList<QPoint> &pts) : pontos(pts) {}

void Poligono::desenhar(QPainter &painter) {
    for (int i = 0; i < pontos.size(); ++i)
        painter.drawLine(pontos[i], pontos[(i + 1) % pontos.size()]);
}

void Poligono::transladar(int dx, int dy) {
    Matriz T = Matriz::translacao(dx, -dy);
    for (QPoint &p : pontos)
        p = T.aplicar(p).toPoint();
}

void Poligono::escalar(double sx, double sy) {
    QPoint centro(0, 0);
    for (const QPoint &p : pontos)
        centro += p;
    centro /= pontos.size();

    Matriz T = Matriz::translacao(-centro.x(), -centro.y());
    Matriz S = Matriz::escala(sx, sy);
    Matriz T_inv = Matriz::translacao(centro.x(), centro.y());
    Matriz transformacao = T_inv * S * T;

    for (QPoint &p : pontos)
        p = transformacao.aplicar(p).toPoint();
}

void Poligono::rotacionar(double anguloGraus, int cx, int cy) {
    Matriz T = Matriz::translacao(-cx, -cy);
    Matriz R = Matriz::rotacao(anguloGraus);
    Matriz T_inv = Matriz::translacao(cx, cy);
    Matriz transformacao = T_inv * R * T;

    for (QPoint &p : pontos)
        p = transformacao.aplicar(p).toPoint();
}

static bool dentro(QPointF p, int borda, const QRectF &clip) {
    switch (borda) {
    case 0: return p.x() >= clip.left();    // LEFT
    case 1: return p.x() <= clip.right();   // RIGHT
    case 2: return p.y() >= clip.top();     // TOP
    case 3: return p.y() <= clip.bottom();  // BOTTOM
    }
    return false;
}

static QPointF intersecao(QPointF p1, QPointF p2, int borda, const QRectF &clip) {
    double x1 = p1.x(), y1 = p1.y();
    double x2 = p2.x(), y2 = p2.y();
    double x, y;

    if (x1 == x2 && y1 == y2) return p1; // ponto único

    switch (borda) {
    case 0:  // LEFT
        x = clip.left();
        y = y1 + (y2 - y1) * (clip.left() - x1) / (x2 - x1);
        return {x, y};
    case 1:  // RIGHT
        x = clip.right();
        y = y1 + (y2 - y1) * (clip.right() - x1) / (x2 - x1);
        return {x, y};
    case 2:  // TOP
        y = clip.top();
        x = x1 + (x2 - x1) * (clip.top() - y1) / (y2 - y1);
        return {x, y};
    case 3:  // BOTTOM
        y = clip.bottom();
        x = x1 + (x2 - x1) * (clip.bottom() - y1) / (y2 - y1);
        return {x, y};
    }
    return {};
}

QVector<QLineF> Poligono::getLinhasClipadas(
    std::function<QPointF(QPointF)> worldToWindow,
    const QRectF &clippingWindow,
    std::function<QPointF(QPointF)> windowToViewport) const
{
    // Passo 1: transformar pontos para coordenadas da window
    QVector<QPointF> wpoints;
    for (const QPoint &p : pontos)
        wpoints.append(worldToWindow(p));

    // Passo 2: aplicar Sutherland-Hodgman
    for (int borda = 0; borda < 4; ++borda) {
        QVector<QPointF> entrada = wpoints;
        wpoints.clear();

        for (int i = 0; i < entrada.size(); ++i) {
            QPointF atual = entrada[i];
            QPointF anterior = entrada[(i + entrada.size() - 1) % entrada.size()];

            bool dent_atual = dentro(atual, borda, clippingWindow);
            bool dent_anter = dentro(anterior, borda, clippingWindow);

            if (dent_atual && dent_anter) {
                wpoints.append(atual);
            } else if (!dent_atual && dent_anter) {
                wpoints.append(intersecao(anterior, atual, borda, clippingWindow));
            } else if (dent_atual && !dent_anter) {
                wpoints.append(intersecao(anterior, atual, borda, clippingWindow));
                wpoints.append(atual);
            }
            // se ambos fora, não adiciona nada
        }
    }

    // Passo 3: converter pontos clipados para viewport e criar linhas
    QVector<QLineF> resultado;

    for (int i = 0; i < wpoints.size(); ++i) {
        QPointF p0 = windowToViewport(wpoints[i]);
        QPointF p1 = windowToViewport(wpoints[(i + 1) % wpoints.size()]);
        resultado.append(QLineF(p0, p1));
    }

    return resultado;
}



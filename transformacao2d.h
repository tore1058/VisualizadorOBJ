#ifndef TRANSFORMACAO2D_H
#define TRANSFORMACAO2D_H

#include <QPointF>
#include <QRect>
#include <QRectF>

struct Transformacao2D {
    QRectF window;   // coordenadas normalizadas, ex: (-1, -1, 2, 2)
    QRect viewport;  // área visível, geralmente rect() do CanvasWidget

    // No seu caso, worldToWindow pode ser identidade:
    QPointF worldToWindow(const QPointF &p) const {
        // Assumindo que o mundo é de [0, viewport.width()] e [0, viewport.height()]
        double xw = (p.x() / viewport.width()) * 2.0 - 1.0;
        double yw = (p.y() / viewport.height()) * 2.0 - 1.0;
        return QPointF(xw, yw);
    }


    QPointF windowToViewport(const QPointF &p) const {
        double xvp = viewport.left() + (p.x() - window.left()) * viewport.width() / window.width();
        double yvp = viewport.top() + (p.y() - window.top()) * viewport.height() / window.height();
        return QPointF(xvp, yvp);
    }
};

#endif // TRANSFORMACAO2D_H

#include "matriz.h"
#include <QtMath>
#include <cmath>

Matriz::Matriz() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = (i == j) ? 1.0 : 0.0;
}

Matriz Matriz::identidade() {
    return Matriz();
}

Matriz Matriz::translacao(double dx, double dy) {
    Matriz t;
    t.m[0][2] = dx;
    t.m[1][2] = dy;
    return t;
}

Matriz Matriz::rotacao(double anguloGraus) {
    Matriz r;
    double rad = qDegreesToRadians(anguloGraus);
    r.m[0][0] = cos(rad);
    r.m[0][1] = -sin(rad);
    r.m[1][0] = sin(rad);
    r.m[1][1] = cos(rad);
    return r;
}

Matriz Matriz::escala(double sx, double sy) {
    Matriz s;
    s.m[0][0] = sx;
    s.m[1][1] = sy;
    return s;
}

Matriz Matriz::operator*(const Matriz &outra) const {
    Matriz resultado;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            resultado.m[i][j] = 0;
            for (int k = 0; k < 3; ++k) {
                resultado.m[i][j] += m[i][k] * outra.m[k][j];
            }
        }
    }
    return resultado;
}

QPointF Matriz::aplicar(const QPointF &p) const {
    double x = m[0][0]*p.x() + m[0][1]*p.y() + m[0][2];
    double y = m[1][0]*p.x() + m[1][1]*p.y() + m[1][2];
    return QPointF(x, y);
}

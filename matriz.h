#ifndef MATRIZ_H
#define MATRIZ_H

#include <QPointF>

class Matriz {
    double m[3][3];

public:
    Matriz();

    static Matriz identidade();
    static Matriz translacao(double dx, double dy);
    static Matriz rotacao(double anguloGraus);
    static Matriz escala(double sx, double sy);

    Matriz operator*(const Matriz &outra) const;
    QPointF aplicar(const QPointF &p) const;
};

#endif // MATRIZ_H

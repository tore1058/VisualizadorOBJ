#ifndef OBJETOOBJ_H
#define OBJETOOBJ_H

#include "objeto.h"
#include <QVector3D>
#include <QMatrix4x4>
#include <QVector>
#include <QPair>
#include <QString>
#include <QLineF>
#include <QRectF>
#include <QPainter>
#include <functional>
#include "camera.h"

class ObjetoObj : public ObjetoGrafico {
    QVector<QVector3D> vertices;
    QVector<QPair<int, int>> arestas;
    QMatrix4x4 transform;
    QVector3D centro;  // Centro geométrico do objeto para rotações em torno de si mesmo
    Camera *camera = nullptr;

public:
    ObjetoObj();
    bool carregar(const QString &caminho);

    void setCamera(Camera *cam) { camera = cam; }

    void rotacionar3D(double angX, double angY);
    void escalar3D(double fator);
    void transladarZ(double dz);

    // Implementações da interface ObjetoGrafico
    void desenhar(QPainter &painter) override;
    void transladar(int dx, int dy) override;
    void escalar(double sx, double sy) override;
    void rotacionar(double anguloGraus, int cx, int cy) override;

    QVector<QLineF> getLinhasClipadas(
        std::function<QPointF(QPointF)> worldToWindow,
        const QRectF &clippingWindow,
        std::function<QPointF(QPointF)> windowToViewport) const override;
};

#endif // OBJETOOBJ_H

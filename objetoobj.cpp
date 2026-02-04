#include "objetoobj.h"
#include <QFile>
#include <QTextStream>
#include <QtMath>
#include <QRegularExpression>

ObjetoObj::ObjetoObj() {
    transform.setToIdentity();
}

bool ObjetoObj::carregar(const QString &caminho) {
    QFile f(caminho);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&f);
    QVector<QVector3D> verts;
    QVector<QPair<int, int>> edges;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.startsWith("v ")) {
            QStringList parts = line.split(QRegularExpression("\\s+"));
            if (parts.size() >= 4) {
                float x = parts[1].toFloat();
                float y = parts[2].toFloat();
                float z = parts[3].toFloat();
                verts.append(QVector3D(x, y, z));
            }
        } else if (line.startsWith("f ") || line.startsWith("l ")) {
            QStringList parts = line.split(QRegularExpression("\\s+"));
            QList<int> idx;

            for (int i = 1; i < parts.size(); ++i) {
                QString p = parts[i];
                QStringList sub = p.split('/');
                bool ok = false;
                int vi = sub[0].toInt(&ok);
                if (!ok) continue;
                vi -= 1;
                idx.append(vi);
            }

            if (idx.size() >= 2) {
                int first = idx[0];
                int prev = idx[0];
                for (int i = 1; i < idx.size(); ++i) {
                    edges.append(qMakePair(prev, idx[i]));
                    prev = idx[i];
                }
                if (line.startsWith("f") && idx.size() > 2)
                    edges.append(qMakePair(prev, first));
            }
        }
    }

    vertices = verts;
    arestas = edges;

    // Calcula o centro geométrico
    if (!vertices.isEmpty()) {
        QVector3D sum(0, 0, 0);
        for (const auto &v : vertices)
            sum += v;
        centro = sum / float(vertices.size());
    } else {
        centro = QVector3D();
    }

    return true;
}

void ObjetoObj::rotacionar3D(double angX, double angY) {
    QVector3D c = transform.map(centro);
    QMatrix4x4 m;
    m.translate(c);
    m.rotate(angX, 1, 0, 0);
    m.rotate(angY, 0, 1, 0);
    m.translate(-c);
    transform = m * transform;
}

void ObjetoObj::escalar3D(double fator) {
    QVector3D c = transform.map(centro);
    QMatrix4x4 s;
    s.translate(c);
    s.scale(fator, fator, fator);
    s.translate(-c);
    transform = s * transform;
}

void ObjetoObj::desenhar(QPainter &) {
    // Implementação vazia por enquanto
}

void ObjetoObj::transladar(int dx, int dy) {
    QMatrix4x4 t;
    t.translate(dx, -dy, 0);  // Atenção ao eixo Y invertido
    transform = t * transform;
}

void ObjetoObj::transladarZ(double dz) {
    QMatrix4x4 t;
    t.translate(0, 0, dz);
    transform = t * transform;
}

void ObjetoObj::escalar(double sx, double sy) {
    QMatrix4x4 s;
    s.scale(sx, sy, 1.0);
    transform = s * transform;
}

void ObjetoObj::rotacionar(double anguloGraus, int cx, int cy) {
    QMatrix4x4 m;
    m.translate(cx, cy, 0);
    m.rotate(anguloGraus, 0, 0, 1);
    m.translate(-cx, -cy, 0);
    transform = m * transform;
}

static int computeOutCode(QPointF p, const QRectF &clip) {
    int code = 0;
    if (p.x() < clip.left())  code |= 1;  // Esquerda
    if (p.x() > clip.right()) code |= 2;  // Direita
    if (p.y() < clip.top())   code |= 4;  // Acima
    if (p.y() > clip.bottom()) code |= 8; // Abaixo
    return code;
}

QVector<QLineF> ObjetoObj::getLinhasClipadas(
    std::function<QPointF(QPointF)> worldToWindow,
    const QRectF &clippingWindow,
    std::function<QPointF(QPointF)> windowToViewport) const {

    QVector<QLineF> resultado;

    QMatrix4x4 projView;
    if (camera) {
        projView = camera->matrix();
    } else {
        projView.perspective(60.0f, 1.0f, 0.1f, 1000.0f);
    }

    for (const auto &edge : arestas) {
        if (edge.first < 0 || edge.first >= vertices.size() ||
            edge.second < 0 || edge.second >= vertices.size())
            continue;

        QVector3D v0 = transform.map(vertices[edge.first]);
        QVector3D v1 = transform.map(vertices[edge.second]);
        QVector4D v0h(v0, 1.0f);
        QVector4D v1h(v1, 1.0f);

        v0h = projView * v0h;
        v1h = projView * v1h;

        if (v0h.w() <= 0.0f || v1h.w() <= 0.0f)
            continue;

        QPointF p0(v0h.x() / v0h.w(), v0h.y() / v0h.w());
        QPointF p1(v1h.x() / v1h.w(), v1h.y() / v1h.w());

        QPointF w0 = worldToWindow(p0);
        QPointF w1 = worldToWindow(p1);

        int out0 = computeOutCode(w0, clippingWindow);
        int out1 = computeOutCode(w1, clippingWindow);
        QPointF cp0 = w0, cp1 = w1;

        while (true) {
            if (!(out0 | out1)) {
                QPointF v0p = windowToViewport(cp0);
                QPointF v1p = windowToViewport(cp1);
                resultado.append(QLineF(v0p, v1p));
                break;
            } else if (out0 & out1) {
                break;
            } else {
                double x, y;
                int outcodeOut = out0 ? out0 : out1;
                double xmin = clippingWindow.left();
                double xmax = clippingWindow.right();
                double ymin = clippingWindow.top();
                double ymax = clippingWindow.bottom();
                double x0 = cp0.x(), y0 = cp0.y();
                double x1_ = cp1.x(), y1_ = cp1.y();

                if (outcodeOut & 8) { // Abaixo
                    x = x0 + (x1_ - x0) * (ymax - y0) / (y1_ - y0);
                    y = ymax;
                } else if (outcodeOut & 4) { // Acima
                    x = x0 + (x1_ - x0) * (ymin - y0) / (y1_ - y0);
                    y = ymin;
                } else if (outcodeOut & 2) { // Direita
                    y = y0 + (y1_ - y0) * (xmax - x0) / (x1_ - x0);
                    x = xmax;
                } else { // Esquerda
                    y = y0 + (y1_ - y0) * (xmin - x0) / (x1_ - x0);
                    x = xmin;
                }

                if (outcodeOut == out0) {
                    cp0 = QPointF(x, y);
                    out0 = computeOutCode(cp0, clippingWindow);
                } else {
                    cp1 = QPointF(x, y);
                    out1 = computeOutCode(cp1, clippingWindow);
                }
            }
        }
    }

    return resultado;
}

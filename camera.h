#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class Camera {
    bool perspectiva;
    float fov;
    float aspect;
    float nearPlane;
    float farPlane;
    QVector3D position;
    QVector3D target;
    QVector3D up;
public:
    Camera();

    void setPerspectiva(bool p);
    bool isPerspectiva() const;

    void setFov(float f);
    float getFov() const;

    void setAspect(float a);
    float getAspect() const;

    void setNear(float n);
    float getNear() const;

    void setFar(float f);
    float getFar() const;

    void setPosition(const QVector3D &pos);
    QVector3D getPosition() const;

    void setTarget(const QVector3D &t);
    QVector3D getTarget() const;

    void setUp(const QVector3D &u);
    QVector3D getUp() const;

    void rotate(double pitch, double yaw);

    QMatrix4x4 matrix() const;
};

#endif // CAMERA_H

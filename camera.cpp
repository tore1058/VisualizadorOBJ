#include "camera.h"
#include <QtMath>

Camera::Camera()
    : perspectiva(false), fov(60.0f), aspect(1.0f), nearPlane(0.1f), farPlane(1000.0f),
      position(0.0f, 0.0f, 15.0f), target(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f) {}

void Camera::setPerspectiva(bool p) { perspectiva = p; }
bool Camera::isPerspectiva() const { return perspectiva; }

void Camera::setFov(float f) { fov = f; }
float Camera::getFov() const { return fov; }

void Camera::setAspect(float a) { aspect = a; }
float Camera::getAspect() const { return aspect; }

void Camera::setNear(float n) { nearPlane = n; }
float Camera::getNear() const { return nearPlane; }

void Camera::setFar(float f) { farPlane = f; }
float Camera::getFar() const { return farPlane; }

void Camera::setPosition(const QVector3D &pos) { position = pos; }
QVector3D Camera::getPosition() const { return position; }

void Camera::setTarget(const QVector3D &t) { target = t; }
QVector3D Camera::getTarget() const { return target; }

void Camera::setUp(const QVector3D &u) { up = u; }
QVector3D Camera::getUp() const { return up; }

void Camera::rotate(double pitch, double yaw) {
    QMatrix4x4 m;
    m.rotate(pitch, 1.0f, 0.0f, 0.0f);
    m.rotate(yaw, 0.0f, 1.0f, 0.0f);
    QVector3D dir = target - position;
    dir = m * dir;
    up = (m * up).normalized();
    target = position + dir;
}

QMatrix4x4 Camera::matrix() const {
    QMatrix4x4 proj;
    if (perspectiva) {
        proj.perspective(fov, aspect, nearPlane, farPlane);
    } else {
        float top = tan(qDegreesToRadians(fov / 2.0f));
        float right = top * aspect;
        proj.ortho(-right, right, -top, top, nearPlane, farPlane);
    }
    QMatrix4x4 view;
    view.lookAt(position, target, up);
    return proj * view;
}

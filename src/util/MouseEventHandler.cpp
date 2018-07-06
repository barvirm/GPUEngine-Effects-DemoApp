//
// Created by makku on 2.10.17.
//

#include <MouseEventHandler.h>
#include <QOpenGLWidget>
#include <iostream>
#include <QDebug>
#include <QtQuick/QQuickWindow>
#include <geUtil/OrbitCamera.h>

app::MouseEventHandler::MouseEventHandler(std::shared_ptr<ge::util::OrbitCamera> &orbCamera) :
        orbitCamera(orbCamera)
{
}

/*
 * Event filer for QOpenGLWidget
 */
bool app::MouseEventHandler::eventFilter(QObject *object, QEvent *event) {

    bool filtered = false;
    switch (event->type()) {
        case QEvent::MouseButtonPress:
            filtered = mouseFilterButtonPress(dynamic_cast<QMouseEvent *>(event));
            break;
        case QEvent::MouseButtonRelease:
            filtered = mouseFilterButtonRelease(dynamic_cast<QMouseEvent *>(event));
            break;
        case QEvent::MouseMove:
            filtered = mouseFilterMove(dynamic_cast<QMouseEvent *>(event), object);
            break;
        case QEvent::Wheel:
            filtered = mouseFilterWheel(dynamic_cast<QWheelEvent *>(event), object);
            break;
        default:
            break;
    }
    if (filtered) {
        return true;
    }
    return QObject::eventFilter(object, event);
}

bool app::MouseEventHandler::mouseFilterButtonPress(QMouseEvent *mouseEvent) {
    mouseMovement.moved = false;
    if (mouseEvent->button() == Qt::LeftButton) {
        mouseButton.cameraRotating = true;
        mouseMovement.oldX = mouseEvent->x();
        mouseMovement.oldY = mouseEvent->y();
        return true;
    } else if (mouseEvent->button() == Qt::RightButton) {
        mouseButton.cameraPanning = true;
        mouseMovement.oldX = mouseEvent->x();
        mouseMovement.oldY = mouseEvent->y();
        return true;
    }
    return false;
}

bool app::MouseEventHandler::mouseFilterButtonRelease(QMouseEvent *mouseEvent) {
    if (mouseEvent->button() == Qt::LeftButton) {
        mouseButton.cameraRotating = false;
        return true;
    } else if (mouseEvent->button() == Qt::RightButton) {
        mouseButton.cameraPanning = false;
        return true;
    }
    return false;
}

bool app::MouseEventHandler::mouseFilterMove(QMouseEvent *mouseEvent, QObject *object) {
    auto *glw = dynamic_cast<QQuickWindow *>(object);
    QSize size = glw->size();
    float dx = (mouseEvent->x() - mouseMovement.oldX) / size.width();
    float dy = (mouseEvent->y() - mouseMovement.oldY) / size.height();
    mouseMovement.oldX = mouseEvent->x();
    mouseMovement.oldY = mouseEvent->y();
    mouseMovement.moved = true;

    if (mouseButton.cameraRotating) {
        glw->update();
        orbitCamera->addAngles(glm::vec2(dy, dx));
        return true;
    } else if (mouseButton.cameraPanning) {
        glw->update();
        orbitCamera->addXYPosition(glm::vec2(-dx, dy));
        return true;
    }
    return false;
}

bool app::MouseEventHandler::mouseFilterWheel(QWheelEvent *wheelEvent, QObject *object) {
    auto *glw = dynamic_cast<QQuickWindow *>(object);
    auto distance = wheelEvent->angleDelta().y() / 120;
    orbitCamera->addDistance(-distance * 1.1f);
    glw->update();
    return true;
}

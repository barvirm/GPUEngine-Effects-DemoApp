#pragma once

#include <memory>
#include <QEvent>
#include <QMouseEvent>
#include <QObject>

namespace ge {
    namespace util {
        class OrbitCamera;
    }
}

namespace app {
    class MouseEventHandler : public QObject {
    Q_OBJECT
    public:
        MouseEventHandler(std::shared_ptr<ge::util::OrbitCamera> &orbitCamera);
        std::shared_ptr<ge::util::OrbitCamera> orbitCamera;

    protected:
        bool eventFilter(QObject *object, QEvent *event);

        struct MouseMovement {
            MouseMovement() :
                    oldX(0),
                    oldY(0),
                    moved(false) {}

            float oldX, oldY;
            bool moved;
        } mouseMovement;

        struct MouseButton {
            MouseButton() :
                    cameraRotating(false),
                    cameraPanning(false),
                    cameraZooming(false) {}

            bool cameraRotating, cameraPanning, cameraZooming;
        } mouseButton;

    private:
        bool mouseFilterButtonPress(QMouseEvent *mouseEvent);
        bool mouseFilterButtonRelease(QMouseEvent *mouseEvent);
        bool mouseFilterMove(QMouseEvent *mouseEvent, QObject *object);
        bool mouseFilterWheel(QWheelEvent *wheelEvent, QObject *object);
    };
}


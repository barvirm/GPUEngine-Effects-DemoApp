#include <iostream>
#include <QGuiApplication>
#include <MouseEventHandler.h>
#include <QQuickWindow>
#include <Renderer.h>
#include <SceneLoader.h>
#include <memory>
#include <MouseEventHandler.h>

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    QQuickWindow qw;

    msg::Renderer renderer(&qw);
    std::shared_ptr<ge::sg::Scene> scene = app::SceneLoader::loadScene(APP_RESOURCES"/models/ship.obj");
    renderer.setScene(scene);

    std::shared_ptr<app::MouseEventHandler> mouseEventHandler(std::make_shared<app::MouseEventHandler>(renderer.orbitCamera));
    app.installEventFilter(mouseEventHandler.get());
    qw.show();


    qw.resize(1200,1000);
    return app.exec();
}

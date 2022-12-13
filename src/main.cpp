#include "mainwindow.h"

#include "settings.h"
#include <QApplication>
#include <QScreen>
#include <iostream>
#include <QSettings>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Projects 5 & 6: Lights, Camera & Action!");
    QCoreApplication::setOrganizationName("CS 1230");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QSurfaceFormat fmt;
    fmt.setVersion(4, 1);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    settings.sceneFilePath = QCoreApplication::arguments().at(1).toStdString();
    settings.objFilePath = QCoreApplication::arguments().at(2).toStdString();
    settings.drawSceneGeometry = QCoreApplication::arguments().at(3).toStdString();
    settings.drawObjMeshes = QCoreApplication::arguments().at(4).toStdString();

    std::cout << "scenefile: " << settings.sceneFilePath << std::endl;
    std::cout << "objfile: " << settings.objFilePath << std::endl;
    std::cout << "drawGeometry: " << settings.drawSceneGeometry << std::endl;
    std::cout << "drawObj: " << settings.drawObjMeshes << std::endl;
    std::cout << "drawShadows: " << settings.drawShadows << std::endl;

    MainWindow w;
    w.initialize();
    w.resize(800, 600);
    int desktopArea = QGuiApplication::primaryScreen()->size().width() *
                      QGuiApplication::primaryScreen()->size().height();
    int widgetArea = w.width() * w.height();
    if (((float)widgetArea / (float)desktopArea) < 0.75f) {
        w.show();
    } else {
        w.showMaximized();
    }

    int return_val = a.exec();
    w.finish();
    return return_val;
}

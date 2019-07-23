// libraries
#include <QDBusConnection>
#include <appimage/desktop_integration/IntegrationManager.h>

// local
#include "ThumbnailerDBusInterface.h"
#include "utils.h"

// generated by `qt5_add_dbus_adaptor` cmake function
#include "thumbnaileradaptor.h"

ThumbnailerDBusInterface::ThumbnailerDBusInterface(QObject* parent) : QObject(parent) {
    new ThumbnailerAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    bool operationSucceed;
    operationSucceed = dbus.registerObject(THUMBNAILER_DBUS_OBJECT_PATH, this);
    if (!operationSucceed)
        qCritical() << "Unable to register d-bus object: " << THUMBNAILER_DBUS_OBJECT_PATH;

    operationSucceed = dbus.registerService(THUMBNAILER_DBUS_INTERFACE_NAME);
    if (!operationSucceed)
        qCritical() << "Unable to register d-bus service: " << THUMBNAILER_DBUS_INTERFACE_NAME;
}

bool ThumbnailerDBusInterface::create(QString appImagePath) {
    QString path = removeUriProtocolFromPath(appImagePath);
    try {
        appimage::desktop_integration::IntegrationManager manager;

        appimage::core::AppImage appImage(path.toStdString());
        manager.generateThumbnails(appImage);
        return true;
    } catch (const std::runtime_error& error) {
        qWarning() << "Unable to generate thumbnails for " << appImagePath << " error: " << error.what();
        return false;
    }
}

bool ThumbnailerDBusInterface::destroy(QString appImagePath) {
    QString path = removeUriProtocolFromPath(appImagePath);
    try {
        appimage::desktop_integration::IntegrationManager manager;
        manager.removeThumbnails(path.toStdString());
        return true;
    } catch (const std::runtime_error& error) {
        qWarning() << "Unable to generate thumbnails for " << appImagePath << " error: " << error.what();
        return false;
    }
}

ThumbnailerDBusInterface::~ThumbnailerDBusInterface() = default;
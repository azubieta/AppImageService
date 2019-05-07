// system
#include <iostream>
#include <fstream>

// libraries
#include <QProcess>
#include <QCoreApplication>
#include <appimage/appimage.h>
#include <XdgUtils/DesktopEntry/DesktopEntry.h>
#include <XdgUtils/DesktopEntry/DesktopEntryStringsValue.h>

// local
#include "LauncherService.h"
#include "LauncherDefines.h"

bool LauncherService::registerApp(const std::string& appImagePath) const {
    try {
        appimage::core::AppImage appImage(appImagePath);
        integrationManager.registerAppImage(appImage);
        registerAppAddRemoveDesktopEntryAction(appImagePath);
        return true;
    } catch (const appimage::core::AppImageError& ex) {
        std::cerr << "Unable to register: " << appImagePath << " : " << ex.what() << std::endl;
    }
    return false;
}

bool LauncherService::unregisterApp(const std::string& appImagePath) const {
    try {
        integrationManager.unregisterAppImage(appImagePath);
        return true;
    } catch (const appimage::core::AppImageError& ex) {
        std::cerr << "Unable to register: " << appImagePath << " : " << ex.what() << std::endl;
    }
    return false;
}

void LauncherService::registerAppAddRemoveDesktopEntryAction(const std::string& appImagePath) const {
    using namespace XdgUtils::DesktopEntry;

    char* desktopFilePath = appimage_registered_desktop_file_path(appImagePath.c_str(), nullptr, false);
    if (desktopFilePath != nullptr) {
        std::ifstream ifstream(desktopFilePath);
        DesktopEntry entry(ifstream);

        std::string actionsString = static_cast<std::string>(entry["Desktop Entry/Actions"]);
        DesktopEntryStringsValue actions(actionsString);

        // Add uninstall action
        actions.append("Remove");
        entry.set("Desktop Entry/Actions", actions.dump());

        entry.set("Desktop Action Remove/Name", "Remove");
        entry.set("Desktop Action Remove/Icon", "application-x-trash");

        // Build Remove Action Exec Value
        auto removeCommand = QString("dbus-send --type=method_call --dest=%1 %2 %3 string:'%4'")
            .arg(LAUNCHER_DBUS_INTERFACE_NAME)
            .arg(LAUNCHER_DBUS_OBJECT_PATH)
            .arg(LAUNCHER_DBUS_METHOD_UNREGISTER_APP)
            .arg(QString::fromStdString(appImagePath));

        entry.set("Desktop Action Remove/Exec", removeCommand.toStdString());
        entry.set("Desktop Action Remove/TryExec", "dbus-send");

        std::ofstream ofstream(desktopFilePath);
        ofstream << entry;
    }
}
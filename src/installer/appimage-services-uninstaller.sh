#!/usr/bin/env bash

# Includes
SCRIPTS_DIR=$(dirname "$0")
source ${SCRIPTS_DIR}/utils/debug-header.sh
source ${SCRIPTS_DIR}/utils/settings.sh

echo "Uninstalling AppImage Services"

# Stop systemd daemon
if [ "$EUID" -ne 0 ]; then
  # Non root installation
  systemctl --user stop appimage-services
  systemctl --user disable appimage-services
else
  # Root installation
  systemctl --user --global stop appimage-services
  systemctl --user --global disable appimage-services
fi

# binfmt is only available when installed as root
if [ "$EUID" -eq 0 ]; then
  echo "Uninstall binfmt"
    update-binfmts --package appimage --remove appimage-type1 $BINDIR/appimage-services-launcher-type-1
    update-binfmts --package appimage --remove appimage-type2 $BINDIR/appimage-services-launcher-type-2

  systemctl restart binfmt-support.service
fi

# Uninstall files
for SERVICE in "Launcher" "Updater" "Inspector"; do
  rm $DBUS_SYSTEM_SERVICES_DIR/org.appimage.Services1.$SERVICE.service
  echo "Removed $DBUS_SYSTEM_SERVICES_DIR/org.appimage.Services1.$SERVICE.service"
done

rm $SYSTEMD_USER_DIR/appimage-services.service
echo "Removed $SYSTEMD_USER_DIR/appimage-services.service"

rm $BINDIR/appimage-services
echo "Removed $BINDIR/appimage-services"

echo "Uninstalling runtimes to $BINDIR"
rm -v $BINDIR/appimage-services-launcher-type-1
rm -v $BINDIR/appimage-services-launcher-type-2

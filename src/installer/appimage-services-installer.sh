#!/usr/bin/env bash

# Includes
SCRIPTS_DIR=`dirname "$0"`
source ${SCRIPTS_DIR}/utils/debug-header.sh
source ${SCRIPTS_DIR}/utils/settings.sh

echo "Intalling binary to $BINDIR/appimage-services"
mkdir -p $BINDIR
cp --force $APPIMAGE $BINDIR/appimage-services


mkdir -p $DBUS_SYSTEM_SERVICES_DIR
for SERVICE in "Launcher" "Updater" "Inspector"; do

echo "Installing dbus service to $DBUS_SYSTEM_SERVICES_DIR/org.appimage.Services1.$SERVICE.service"
cat > "$DBUS_SYSTEM_SERVICES_DIR/org.appimage.Services1.$SERVICE.service" <<EOF
[D-BUS Service]
Name=org.appimage.Services1.$SERVICE
Exec=/bin/false
User=root
SystemdService=appimage-services.service
EOF

done

echo "Installing systemd service to $SYSTEMD_USER_DIR/appimage-services.service"
mkdir -p $SYSTEMD_USER_DIR
cat > "$SYSTEMD_USER_DIR/appimage-services.service" <<EOF
[Unit]
Description=AppImage Services

[Service]
Type=dbus
ExecStart=$BINDIR/appimage-services
Restart=on-failure
RestartSec=10
BusName=org.appimage.Services1.Launcher

[Install]
WantedBy=default.target
EOF

if [ "$EUID" -ne 0 ]; then
    # Non root installation
    systemctl --user daemon-reload
    systemctl --user enable appimage-services
    systemctl --user restart appimage-services
else
    # Root installation
    systemctl daemon-reload
    systemctl --user --global enable appimage-services
    systemctl --user --global restart appimage-services
fi

if [ ! $KEEP_INSTALLER ]; then
    rm --verbose $APPIMAGE
fi

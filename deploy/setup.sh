#!/bin/bash

CURRENT_DIR=$(pwd)

chmod +x $CURRENT_DIR/SlamInterface.AppImage

echo "[Desktop Entry]
Type=Application
Name=SlamInterface
Exec=$CURRENT_DIR/SlamInterface.AppImage
Icon=$CURRENT_DIR/robot.svg
Terminal=false
Name[zh_CN]=SlamInterface.desktop" >>"$CURRENT_DIR/SlamInterface.desktop"

mv $CURRENT_DIR/SlamInterface.desktop /usr/share/applications
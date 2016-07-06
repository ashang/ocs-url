#!/bin/sh

# Uninstall

sudo rm /usr/bin/xdgurl
sudo rm /usr/share/applications/xdgurl.desktop
sudo update-desktop-database -q /usr/share/applications

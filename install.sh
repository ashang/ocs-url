#!/bin/sh

# Install

cd `dirname $0`

sudo apt-get install python
sudo apt-get install python-tk
sudo apt-get install desktop-file-utils
sudo install -m 755 src/xdgurl.py /usr/bin/xdgurl
sudo install -m 644 src/xdgurl.desktop /usr/share/applications/xdgurl.desktop
sudo update-desktop-database -q /usr/share/applications

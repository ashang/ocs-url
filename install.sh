#!/bin/bash

# Install

cd `dirname $0`

sudo apt-get update
sudo apt-get install python-tk
sudo install -m 755 src/xdgurl.py /usr/bin/xdgurl
sudo install -m 644 src/xdgurl.desktop /usr/share/applications/xdgurl.desktop
sudo update-desktop-database

#!/bin/sh

cd `dirname $0`

build_ubuntu() {
    sudo apt install build-essential qt5-default libqt5svg5-dev qtdeclarative5-dev devscripts debhelper fakeroot

    mkdir ./build
    cp -r ../src ./build/
    cp ../*.pro ./build/
    cp ../*.pri ./build/
    cp -r ./ubuntu/debian ./build/
    cd ./build
    debuild -uc -us -b
}

build_fedora() {
    su -c 'dnf install make automake gcc gcc-c++ libtool qt5-qtbase-devel qt5-qtsvg-devel qt5-qtdeclarative-devel rpm-build'

    tar -czvf /tmp/xdgurl.tar.gz ../../xdgurl
    mkdir ./build
    mkdir ./build/SOURCES
    mkdir ./build/SPECS
    mv /tmp/xdgurl.tar.gz ./build/SOURCES/
    cp ./fedora/xdgurl.spec ./build/SPECS/
    rpmbuild --define '_topdir '`pwd`'/build' -bb ./build/SPECS/xdgurl.spec
}

build_arch() {
    sudo pacman -S base-devel qt5-base qt5-svg qt5-declarative qt5-quickcontrols

    tar -czvf /tmp/xdgurl.tar.gz ../../xdgurl
    mkdir ./build
    mv /tmp/xdgurl.tar.gz ./build/
    cp ./arch/PKGBUILD ./build/
    cd ./build
    updpkgsums
    makepkg -s
}

if [ $1 ] && [ $1 = 'ubuntu' ]; then
    build_ubuntu
elif [ $1 ] && [ $1 = 'fedora' ]; then
    build_fedora
elif [ $1 ] && [ $1 = 'arch' ]; then
    build_arch
else
    echo 'sh build.sh [ubuntu|fedora|arch]'
fi

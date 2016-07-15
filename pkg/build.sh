#!/bin/sh

cd `dirname $0`

build_ubuntu() {
    mkdir ./build
    cp -r ../src ./build/
    cp ../Makefile ./build/
    cp -r ./ubuntu/debian ./build/
    cd ./build
    debuild -uc -us -b
}

build_fedora() {
    tar -czvf /tmp/xdgurl.tar.gz ../../xdgurl
    mkdir -p ./build/SOURCES
    mkdir -p ./build/SPECS
    mv /tmp/xdgurl.tar.gz ./build/SOURCES/
    cp ./fedora/xdgurl.spec ./build/SPECS/
    rpmbuild --define '_topdir '`pwd`'/build' -bb ./build/SPECS/xdgurl.spec
}

build_arch() {
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

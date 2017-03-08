#!/bin/bash

################################################################################
# This is wrapper script for build.sh use from inside docker container
################################################################################

PKGUSER='pkgbuilder'

BUILDTYPE=''
if [ "${1}" ]; then
    BUILDTYPE="${1}"
fi

PROJDIR="$(cd "$(dirname "${0}")/../" && pwd)"

BUILDSCRIPT="${PROJDIR}/scripts/build.sh"

build_ubuntu() {
    # docker-image: ubuntu:14.04
    apt update -qq
    apt -y install git curl
    apt -y install build-essential qt5-default libqt5svg5-dev qtdeclarative5-dev
    apt -y install devscripts debhelper fakeroot

    useradd -m ${PKGUSER}
    export HOME="/home/${PKGUSER}"
    chown -R ${PKGUSER}:${PKGUSER} "${PROJDIR}"

    su -c "sh "${BUILDSCRIPT}" ${BUILDTYPE}" ${PKGUSER}
}

build_fedora() {
    # docker-image: fedora:20
    yum -y install git curl
    yum -y install make automake gcc gcc-c++ libtool qt5-qtbase-devel qt5-qtsvg-devel qt5-qtdeclarative-devel
    yum -y install rpm-build
    # docker-image: fedora:22
    #dnf -y install git curl
    #dnf -y install make automake gcc gcc-c++ libtool qt5-qtbase-devel qt5-qtsvg-devel qt5-qtdeclarative-devel
    #dnf -y install rpm-build

    useradd -m ${PKGUSER}
    export HOME="/home/${PKGUSER}"
    chown -R ${PKGUSER}:${PKGUSER} "${PROJDIR}"

    su -c "sh "${BUILDSCRIPT}" ${BUILDTYPE}" ${PKGUSER}
}

build_archlinux() {
    # docker-image: base/archlinux:latest
    pacman -Syu --noconfirm
    pacman -S --noconfirm git curl
    pacman -S --noconfirm base-devel qt5-base qt5-svg qt5-declarative qt5-quickcontrols

    useradd -m ${PKGUSER}
    export HOME="/home/${PKGUSER}"
    chown -R ${PKGUSER}:${PKGUSER} "${PROJDIR}"

    su -c "sh "${BUILDSCRIPT}" ${BUILDTYPE}" ${PKGUSER}
}

build_snap() {
    # docker-image: ubuntu:16.04
    apt update -qq
    apt -y install git curl
    apt -y install build-essential qt5-default libqt5svg5-dev qtdeclarative5-dev
    apt -y install snapcraft

    useradd -m ${PKGUSER}
    export HOME="/home/${PKGUSER}"
    chown -R ${PKGUSER}:${PKGUSER} "${PROJDIR}"

    su -c "sh "${BUILDSCRIPT}" ${BUILDTYPE}" ${PKGUSER}
}

build_appimage() {
    # docker-image: ubuntu:14.04
    apt update -qq
    apt -y install git curl
    apt -y install build-essential qt5-default libqt5svg5-dev qtdeclarative5-dev
    apt -y install fuse zsync desktop-file-utils

    modprobe fuse

    useradd -m ${PKGUSER}
    export HOME="/home/${PKGUSER}"
    chown -R ${PKGUSER}:${PKGUSER} "${PROJDIR}"

    su -c "sh "${BUILDSCRIPT}" ${BUILDTYPE}" ${PKGUSER}
}

if [ "${BUILDTYPE}" = 'ubuntu' ]; then
    build_ubuntu
elif [ "${BUILDTYPE}" = 'fedora' ]; then
    build_fedora
elif [ "${BUILDTYPE}" = 'archlinux' ]; then
    build_archlinux
elif [ "${BUILDTYPE}" = 'snap' ]; then
    build_snap
elif [ "${BUILDTYPE}" = 'appimage' ]; then
    build_appimage
else
    echo "sh $(basename "${0}") [ubuntu|fedora|archlinux|snap|appimage]"
    exit 1
fi

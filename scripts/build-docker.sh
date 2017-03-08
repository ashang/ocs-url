#!/bin/bash

################################################################################
# This is wrapper script for build.sh use from inside docker container
################################################################################

PKGNAME='ocs-url'

PKGUSER='pkgbuilder'

BUILDTYPE=''
if [ "${1}" ]; then
    BUILDTYPE="${1}"
fi

PROJDIR="$(cd "$(dirname "${0}")/../" && pwd)"

BUILDSCRIPT="${PROJDIR}/scripts/build.sh"

transfer_file() {
    filepath="${1}"
    if [ -f "${filepath}" ]; then
        filename="$(basename "${filepath}")"
        echo "Uploading ${filename}"
        curl -T "${filepath}" "https://transfer.sh/${filename}"
    fi
}

build_ubuntu() {
    # docker-image: ubuntu:14.04
    apt update -qq
    apt -y install build-essential qt5-default libqt5svg5-dev qtdeclarative5-dev
    apt -y install git devscripts debhelper fakeroot
    apt -y install curl

    useradd -m ${PKGUSER}
    export HOME="/home/${PKGUSER}"
    chown -R ${PKGUSER}:${PKGUSER} "${PROJDIR}"

    su -c "sh "${BUILDSCRIPT}" ${BUILDTYPE}" ${PKGUSER}

    transfer_file "$(find "${PROJDIR}/build_*" -type f -name "${PKGNAME}*.deb")"
}

build_fedora() {
    # docker-image: fedora:20
    yum -y install make automake gcc gcc-c++ libtool qt5-qtbase-devel qt5-qtsvg-devel qt5-qtdeclarative-devel
    yum -y install git rpm-build
    yum -y install curl
    # docker-image: fedora:22
    #dnf -y install make automake gcc gcc-c++ libtool qt5-qtbase-devel qt5-qtsvg-devel qt5-qtdeclarative-devel
    #dnf -y install git rpm-build
    #dnf -y install curl

    useradd -m ${PKGUSER}
    export HOME="/home/${PKGUSER}"
    chown -R ${PKGUSER}:${PKGUSER} "${PROJDIR}"

    su -c "sh "${BUILDSCRIPT}" ${BUILDTYPE}" ${PKGUSER}

    transfer_file "$(find "${PROJDIR}/build_*" -type f -name "${PKGNAME}*.rpm")"
}

build_archlinux() {
    # docker-image: base/archlinux:latest
    pacman -Syu --noconfirm
    pacman -S --noconfirm base-devel qt5-base qt5-svg qt5-declarative qt5-quickcontrols
    pacman -S --noconfirm git
    pacman -S --noconfirm curl

    useradd -m ${PKGUSER}
    export HOME="/home/${PKGUSER}"
    chown -R ${PKGUSER}:${PKGUSER} "${PROJDIR}"

    su -c "sh "${BUILDSCRIPT}" ${BUILDTYPE}" ${PKGUSER}

    transfer_file "$(find "${PROJDIR}/build_*" -type f -name "${PKGNAME}*.pkg.tar.xz")"
}

build_snap() {
    # docker-image: ubuntu:16.04
    apt update -qq
    apt -y install build-essential qt5-default libqt5svg5-dev qtdeclarative5-dev
    apt -y install git snapcraft
    apt -y install curl

    useradd -m ${PKGUSER}
    export HOME="/home/${PKGUSER}"
    chown -R ${PKGUSER}:${PKGUSER} "${PROJDIR}"

    su -c "sh "${BUILDSCRIPT}" ${BUILDTYPE}" ${PKGUSER}

    transfer_file "$(find "${PROJDIR}/build_*" -type f -name "${PKGNAME}*.snap")"
}

build_appimage() {
    # docker-image: ubuntu:14.04
    apt update -qq
    apt -y install build-essential qt5-default libqt5svg5-dev qtdeclarative5-dev
    apt -y install git fuse zsync desktop-file-utils
    apt -y install curl

    modprobe fuse

    useradd -m ${PKGUSER}
    export HOME="/home/${PKGUSER}"
    chown -R ${PKGUSER}:${PKGUSER} "${PROJDIR}"

    su -c "sh "${BUILDSCRIPT}" ${BUILDTYPE}" ${PKGUSER}

    transfer_file "$(find "${PROJDIR}/build_*" -type f -name "${PKGNAME}*.AppImage")"
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

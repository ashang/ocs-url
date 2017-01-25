#!/bin/bash

PKGNAME='xdgurl'

PROJDIR="$(cd "$(dirname "${0}")/../" && pwd)"

BUILDDIR="${PROJDIR}/pkg/build"

BUILDTYPE=''
if [ "${1}" ]; then
    BUILDTYPE="${1}"
fi

TREEISH='HEAD'
if [ "${2}" ]; then
    TREEISH="${2}"
fi

export_source() {
    destdir="${BUILDDIR}"
    if [ "${1}" ]; then
        destdir="${1}"
    fi
    $(cd "${PROJDIR}" && git archive --prefix="${PKGNAME}/" --output="${destdir}/${PKGNAME}.tar.gz" "${TREEISH}")
}

build_ubuntu() {
    #sudo apt install build-essential qt5-default libqt5svg5-dev qtdeclarative5-dev devscripts debhelper fakeroot

    cd "${PROJDIR}"

    mkdir -p "${BUILDDIR}"
    export_source "${BUILDDIR}"
    tar -xzvf "${BUILDDIR}/${PKGNAME}.tar.gz" -C "${BUILDDIR}"
    cp -r "${PROJDIR}/pkg/ubuntu/debian" "${BUILDDIR}/${PKGNAME}"

    cd "${BUILDDIR}/${PKGNAME}"
    debuild -uc -us -b
}

build_fedora() {
    #sudo dnf install make automake gcc gcc-c++ libtool qt5-qtbase-devel qt5-qtsvg-devel qt5-qtdeclarative-devel rpm-build

    cd "${PROJDIR}"

    mkdir -p "${BUILDDIR}"
    mkdir "${BUILDDIR}/SOURCES"
    mkdir "${BUILDDIR}/SPECS"
    export_source "${BUILDDIR}/SOURCES"
    cp "${PROJDIR}/pkg/fedora/xdgurl.spec" "${BUILDDIR}/SPECS"

    rpmbuild --define "_topdir ${BUILDDIR}" -bb "${BUILDDIR}/SPECS/xdgurl.spec"
}

build_arch() {
    #sudo pacman -S base-devel qt5-base qt5-svg qt5-declarative qt5-quickcontrols

    cd "${PROJDIR}"

    mkdir -p "${BUILDDIR}"
    export_source "${BUILDDIR}"
    cp "${PROJDIR}/pkg/arch/PKGBUILD" "${BUILDDIR}"

    cd "${BUILDDIR}"
    updpkgsums
    makepkg -s
}

build_appimage() {
    #sudo add-apt-repository ppa:beineri/opt-qt57-xenial
    #sudo apt update

    #sudo apt install build-essential fuse zsync desktop-file-utils
    #sudo apt install qt57base qt57svg qt57declarative qt57quickcontrols
    # Replace linuxdeployqt download URL to official download URL when the stable version released
    #curl -L -o linuxdeployqt https://dl.dropboxusercontent.com/u/150776/temp/linuxdeployqt-799f704-x86-64.appimage
    #sudo install -m 755 -p linuxdeployqt /usr/local/bin/linuxdeployqt

    #sudo modprobe fuse
    #source /opt/qt57/bin/qt57-env.sh

    cd "${PROJDIR}"

    export VERSION="$(git describe --always)"
    mkdir -p "${BUILDDIR}"
    export_source "${BUILDDIR}"
    tar -xzvf "${BUILDDIR}/${PKGNAME}.tar.gz" -C "${BUILDDIR}"

    cd "${BUILDDIR}/${PKGNAME}"
    #qmake
    /opt/qt57/bin/qmake
    make
    strip ./xdgurl

    cd "${BUILDDIR}"
    mkdir -p "${BUILDDIR}/${PKGNAME}.AppDir/usr/bin"
    install -m 755 -p "${BUILDDIR}/${PKGNAME}/xdgurl" "${BUILDDIR}/${PKGNAME}.AppDir/xdgurl"
    install -m 644 -p "${BUILDDIR}/${PKGNAME}/src/desktop/xdgurl.desktop" "${BUILDDIR}/${PKGNAME}.AppDir/xdgurl.desktop"
    install -m 644 -p "${BUILDDIR}/${PKGNAME}/src/desktop/xdgurl.svg" "${BUILDDIR}/${PKGNAME}.AppDir/xdgurl.svg"
    install -m 755 -p /usr/bin/update-desktop-database "${BUILDDIR}/${PKGNAME}.AppDir/usr/bin/update-desktop-database"
    install -m 755 -p /usr/bin/desktop-file-validate "${BUILDDIR}/${PKGNAME}.AppDir/usr/bin/desktop-file-validate"
    install -m 755 -p /usr/bin/desktop-file-install "${BUILDDIR}/${PKGNAME}.AppDir/usr/bin/desktop-file-install"
    linuxdeployqt "${BUILDDIR}/${PKGNAME}.AppDir/xdgurl" -qmldir="${BUILDDIR}/${PKGNAME}/src/app/qml" -verbose=2 -bundle-non-qt-libs # https://github.com/probonopd/linuxdeployqt/issues/25
    linuxdeployqt "${BUILDDIR}/${PKGNAME}.AppDir/xdgurl" -qmldir="${BUILDDIR}/${PKGNAME}/src/app/qml" -verbose=2 -bundle-non-qt-libs # twice because of #25
    rm "${BUILDDIR}/${PKGNAME}.AppDir/AppRun"
    install -m 755 -p "${BUILDDIR}/${PKGNAME}/src/desktop/appimage-desktopintegration" "${BUILDDIR}/${PKGNAME}.AppDir/AppRun"
    linuxdeployqt --appimage-extract
    ./squashfs-root/usr/bin/appimagetool "${BUILDDIR}/${PKGNAME}.AppDir"
}

if [ "${BUILDTYPE}" = 'ubuntu' ]; then
    build_ubuntu
elif [ "${BUILDTYPE}" = 'fedora' ]; then
    build_fedora
elif [ "${BUILDTYPE}" = 'arch' ]; then
    build_arch
elif [ "${BUILDTYPE}" = 'appimage' ]; then
    build_appimage
else
    echo "sh $(basename "${0}") [ubuntu|fedora|arch|appimage] [tree_ish]"
fi

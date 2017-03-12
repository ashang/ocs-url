#!/bin/bash

################################################################################
# This is utility script to build distribution packages
################################################################################

PKGNAME='ocs-url'

BUILDTYPE=''
if [ "${1}" ]; then
    BUILDTYPE="${1}"
fi

PROJDIR="$(cd "$(dirname "${0}")/../" && pwd)"

BUILDVER="$(cd "${PROJDIR}" && git describe --always)"

BUILDDIR="${PROJDIR}/build_${PKGNAME}_${BUILDVER}_${BUILDTYPE}"

SRCARCHIVE="${BUILDDIR}/${PKGNAME}.tar.gz"

export_srcarchive() {
    filepath="${1}"
    $(cd "${PROJDIR}" && git archive --prefix="${PKGNAME}/" --output="${filepath}" HEAD)
}

build_ubuntu() {
    cd "${PROJDIR}"
    mkdir -p "${BUILDDIR}"
    export_srcarchive "${SRCARCHIVE}"

    tar -xzvf "${SRCARCHIVE}" -C "${BUILDDIR}"
    cp -r "${PROJDIR}/pkg/ubuntu/debian" "${BUILDDIR}/${PKGNAME}"
    cd "${BUILDDIR}/${PKGNAME}"
    debuild -uc -us -b
}

build_fedora() {
    cd "${PROJDIR}"
    mkdir -p "${BUILDDIR}"
    export_srcarchive "${SRCARCHIVE}"

    mkdir "${BUILDDIR}/SOURCES"
    mkdir "${BUILDDIR}/SPECS"
    mv "${SRCARCHIVE}" "${BUILDDIR}/SOURCES"
    cp "${PROJDIR}/pkg/fedora/${PKGNAME}.spec" "${BUILDDIR}/SPECS"
    rpmbuild --define "_topdir ${BUILDDIR}" -bb "${BUILDDIR}/SPECS/${PKGNAME}.spec"
}

build_opensuse() {
    cd "${PROJDIR}"
    mkdir -p "${BUILDDIR}"
    export_srcarchive "${SRCARCHIVE}"

    mkdir "${BUILDDIR}/SOURCES"
    mkdir "${BUILDDIR}/SPECS"
    mv "${SRCARCHIVE}" "${BUILDDIR}/SOURCES"
    cp "${PROJDIR}/pkg/opensuse/${PKGNAME}.spec" "${BUILDDIR}/SPECS"
    rpmbuild --define "_topdir ${BUILDDIR}" -bb "${BUILDDIR}/SPECS/${PKGNAME}.spec"
}

build_archlinux() {
    cd "${PROJDIR}"
    mkdir -p "${BUILDDIR}"
    export_srcarchive "${SRCARCHIVE}"

    cp "${PROJDIR}/pkg/archlinux/PKGBUILD" "${BUILDDIR}"
    cd "${BUILDDIR}"
    updpkgsums
    makepkg -s
}

build_snap() {
    cd "${PROJDIR}"
    mkdir -p "${BUILDDIR}"
    export_srcarchive "${SRCARCHIVE}"

    tar -xzvf "${SRCARCHIVE}" -C "${BUILDDIR}"
    cp "${PROJDIR}/pkg/snap/snapcraft.yaml" "${BUILDDIR}/${PKGNAME}"
    cp -r "${PROJDIR}/pkg/snap/setup" "${BUILDDIR}/${PKGNAME}"
    cd "${BUILDDIR}/${PKGNAME}"
    snapcraft
}

build_appimage() {
    cd "${PROJDIR}"
    mkdir -p "${BUILDDIR}"
    export_srcarchive "${SRCARCHIVE}"

    tar -xzvf "${SRCARCHIVE}" -C "${BUILDDIR}"
    cd "${BUILDDIR}/${PKGNAME}"
    sh scripts/import.sh
    qmake
    make
    strip "${PKGNAME}"

    cd "${BUILDDIR}"

    # Replace linuxdeployqt download URL to official download URL when the stable version released
    curl -L -o linuxdeployqt https://dl.dropboxusercontent.com/u/150776/temp/linuxdeployqt-799f704-x86-64.appimage
    chmod 755 linuxdeployqt

    mkdir -p "${BUILDDIR}/${PKGNAME}.AppDir/usr/bin"
    install -m 755 -p "${BUILDDIR}/${PKGNAME}/${PKGNAME}" "${BUILDDIR}/${PKGNAME}.AppDir/${PKGNAME}"
    install -m 644 -p "${BUILDDIR}/${PKGNAME}/desktop/${PKGNAME}.desktop" "${BUILDDIR}/${PKGNAME}.AppDir/${PKGNAME}.desktop"
    install -m 644 -p "${BUILDDIR}/${PKGNAME}/desktop/${PKGNAME}.svg" "${BUILDDIR}/${PKGNAME}.AppDir/${PKGNAME}.svg"
    install -m 755 -p /usr/bin/update-desktop-database "${BUILDDIR}/${PKGNAME}.AppDir/usr/bin/update-desktop-database"
    install -m 755 -p /usr/bin/desktop-file-validate "${BUILDDIR}/${PKGNAME}.AppDir/usr/bin/desktop-file-validate"
    install -m 755 -p /usr/bin/desktop-file-install "${BUILDDIR}/${PKGNAME}.AppDir/usr/bin/desktop-file-install"
    ./linuxdeployqt "${BUILDDIR}/${PKGNAME}.AppDir/${PKGNAME}" -qmldir="${BUILDDIR}/${PKGNAME}/app/qml" -verbose=2 -bundle-non-qt-libs # https://github.com/probonopd/linuxdeployqt/issues/25
    ./linuxdeployqt "${BUILDDIR}/${PKGNAME}.AppDir/${PKGNAME}" -qmldir="${BUILDDIR}/${PKGNAME}/app/qml" -verbose=2 -bundle-non-qt-libs # twice because of #25
    rm "${BUILDDIR}/${PKGNAME}.AppDir/AppRun"
    install -m 755 -p "${BUILDDIR}/${PKGNAME}/pkg/appimage/appimage-desktopintegration_${PKGNAME}" "${BUILDDIR}/${PKGNAME}.AppDir/AppRun"
    ./linuxdeployqt --appimage-extract
    ./squashfs-root/usr/bin/appimagetool "${BUILDDIR}/${PKGNAME}.AppDir"
}

if [ "${BUILDTYPE}" = 'ubuntu' ]; then
    build_ubuntu
elif [ "${BUILDTYPE}" = 'fedora' ]; then
    build_fedora
elif [ "${BUILDTYPE}" = 'opensuse' ]; then
    build_opensuse
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

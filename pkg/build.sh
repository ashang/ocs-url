#!/bin/bash

################################################################################
# This is utility script to make distribution packages with CI/CD pipelines
################################################################################

PKGNAME='ocs-url'

PROJDIR="$(cd "$(dirname "${0}")/../" && pwd)"

BUILDTYPE=''
if [ "${1}" ]; then
    BUILDTYPE="${1}"
fi

TREEISH='HEAD'
if [ "${2}" ]; then
    TREEISH="${2}"
fi

BUILDDIR="${PROJDIR}/pkg/build_${TREEISH}"

SRCARCHIVE="${BUILDDIR}/${PKGNAME}.tar.gz"

################################################################################
# Utility functions
################################################################################
export_srcarchive() {
    filepath="${1}"
    $(cd "${PROJDIR}" && git archive --prefix="${PKGNAME}/" --output="${filepath}" "${TREEISH}")
}

transfer_file() {
    filepath="${1}"
    if [ -f "${filepath}" ]; then
        filename="$(basename "${filepath}")"
        curl -T "${filepath}" "https://transfer.sh/${filename}"
    fi
}

################################################################################
# ubuntu
################################################################################
pre_ubuntu() {
    cd "${PROJDIR}"
    mkdir -p "${BUILDDIR}"
    export_srcarchive "${SRCARCHIVE}"
}

build_ubuntu() {
    tar -xzvf "${SRCARCHIVE}" -C "${BUILDDIR}"
    cp -r "${PROJDIR}/pkg/ubuntu/debian" "${BUILDDIR}/${PKGNAME}"
    cd "${BUILDDIR}/${PKGNAME}"
    debuild -uc -us -b
}

post_ubuntu() {
    transfer_file "$(find ${BUILDDIR} -type f -name "${PKGNAME}*.deb")"
}

################################################################################
# fedora
################################################################################
pre_fedora() {
    cd "${PROJDIR}"
    mkdir -p "${BUILDDIR}"
    export_srcarchive "${SRCARCHIVE}"
}

build_fedora() {
    mkdir "${BUILDDIR}/SOURCES"
    mkdir "${BUILDDIR}/SPECS"
    mv "${SRCARCHIVE}" "${BUILDDIR}/SOURCES"
    cp "${PROJDIR}/pkg/fedora/${PKGNAME}.spec" "${BUILDDIR}/SPECS"
    rpmbuild --define "_topdir ${BUILDDIR}" -bb "${BUILDDIR}/SPECS/${PKGNAME}.spec"
}

post_fedora() {
    transfer_file "$(find ${BUILDDIR} -type f -name "${PKGNAME}*.rpm")"
}

################################################################################
# archlinux
################################################################################
pre_archlinux() {
    cd "${PROJDIR}"
    mkdir -p "${BUILDDIR}"
    export_srcarchive "${SRCARCHIVE}"
}

build_archlinux() {
    cp "${PROJDIR}/pkg/archlinux/PKGBUILD" "${BUILDDIR}"
    cd "${BUILDDIR}"
    updpkgsums
    makepkg -s
}

post_archlinux() {
    transfer_file "$(find ${BUILDDIR} -type f -name "${PKGNAME}*.pkg.tar.xz")"
}

################################################################################
# snap
################################################################################
pre_snap() {
    cd "${PROJDIR}"
    mkdir -p "${BUILDDIR}"
    export_srcarchive "${SRCARCHIVE}"
}

build_snap() {
    tar -xzvf "${SRCARCHIVE}" -C "${BUILDDIR}"
    cp "${PROJDIR}/pkg/snap/snapcraft.yaml" "${BUILDDIR}/${PKGNAME}"
    cp -r "${PROJDIR}/pkg/snap/setup" "${BUILDDIR}/${PKGNAME}"
    cd "${BUILDDIR}/${PKGNAME}"
    snapcraft
}

post_snap() {
    transfer_file "$(find ${BUILDDIR} -type f -name "${PKGNAME}*.snap")"
}

################################################################################
# appimage
################################################################################
pre_appimage() {
    cd "${PROJDIR}"
    mkdir -p "${BUILDDIR}"
    export_srcarchive "${SRCARCHIVE}"
    export VERSION="$(git describe --always)"
}

build_appimage() {
    tar -xzvf "${SRCARCHIVE}" -C "${BUILDDIR}"
    cd "${BUILDDIR}/${PKGNAME}"
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

post_appimage() {
    transfer_file "$(find ${BUILDDIR} -type f -name "${PKGNAME}*.AppImage")"
}

################################################################################
# Make package
################################################################################
if [ "${BUILDTYPE}" = 'ubuntu' ]; then
    pre_ubuntu && build_ubuntu && post_ubuntu
elif [ "${BUILDTYPE}" = 'fedora' ]; then
    pre_fedora && build_fedora && post_fedora
elif [ "${BUILDTYPE}" = 'archlinux' ]; then
    pre_archlinux && build_archlinux && post_archlinux
elif [ "${BUILDTYPE}" = 'snap' ]; then
    pre_snap && build_snap && post_snap
elif [ "${BUILDTYPE}" = 'appimage' ]; then
    pre_appimage && build_appimage && post_appimage
else
    echo "sh $(basename "${0}") [ubuntu|fedora|archlinux|snap|appimage] [tree_ish]"
    exit 1
fi

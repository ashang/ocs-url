#!/bin/sh

TMPDIR=/tmp
PKGDIR=`cd $(dirname $0) && pwd`
BUILDDIR=$PKGDIR/build
PROJDIR=`dirname $PKGDIR`
PROJNAME=`basename $PROJDIR`
PARENTDIR=`dirname $PROJDIR`

build_ubuntu() {
    #sudo apt install build-essential qt5-default libqt5svg5-dev qtdeclarative5-dev devscripts debhelper fakeroot

    cd $PARENTDIR
    tar -czvf $TMPDIR/$PROJNAME.tar.gz --exclude ".git" $PROJNAME

    mkdir $BUILDDIR

    mv $TMPDIR/$PROJNAME.tar.gz $BUILDDIR
    tar -xzvf $BUILDDIR/$PROJNAME.tar.gz -C $BUILDDIR
    cp -r $PKGDIR/ubuntu/debian $BUILDDIR/$PROJNAME

    cd $BUILDDIR/$PROJNAME
    debuild -uc -us -b
}

build_fedora() {
    #sudo dnf install make automake gcc gcc-c++ libtool qt5-qtbase-devel qt5-qtsvg-devel qt5-qtdeclarative-devel rpm-build

    cd $PARENTDIR
    tar -czvf $TMPDIR/$PROJNAME.tar.gz --exclude ".git" $PROJNAME

    mkdir $BUILDDIR
    mkdir $BUILDDIR/SOURCES
    mkdir $BUILDDIR/SPECS

    mv $TMPDIR/$PROJNAME.tar.gz $BUILDDIR/SOURCES
    cp $PKGDIR/fedora/xdgurl.spec $BUILDDIR/SPECS

    rpmbuild --define "_topdir $BUILDDIR" -bb $BUILDDIR/SPECS/xdgurl.spec
}

build_arch() {
    #sudo pacman -S base-devel qt5-base qt5-svg qt5-declarative qt5-quickcontrols

    cd $PARENTDIR
    tar -czvf $TMPDIR/$PROJNAME.tar.gz --exclude ".git" $PROJNAME

    mkdir $BUILDDIR

    mv $TMPDIR/$PROJNAME.tar.gz $BUILDDIR
    cp $PKGDIR/arch/PKGBUILD $BUILDDIR

    cd $BUILDDIR
    updpkgsums
    makepkg -s
}

build_appimage() {
    #sudo add-apt-repository ppa:beineri/opt-qt57-xenial
    #sudo apt update
    #sudo apt install build-essential fuse desktop-file-utils
    #sudo apt install qt57base qt57svg qt57quickcontrols qt57quickcontrols2
    #curl https://transfer.sh/DHN4B/linuxdeployqt-799f704-x86-64.appimage -o linuxdeployqt.appimage
    #sudo install -m 755 -p linuxdeployqt.appimage /usr/local/bin/linuxdeployqt.appimage
    #sudo modprobe fuse
    #source /opt/qt57/bin/qt57-env.sh

    cd $PROJDIR
    export VERSION=$(git describe --always)

    cd $PARENTDIR
    tar -czvf $TMPDIR/$PROJNAME.tar.gz --exclude ".git" $PROJNAME

    mkdir $BUILDDIR

    mv $TMPDIR/$PROJNAME.tar.gz $BUILDDIR
    tar -xzvf $BUILDDIR/$PROJNAME.tar.gz -C $BUILDDIR

    cd $BUILDDIR/$PROJNAME
    /opt/qt57/bin/qmake
    make
    strip xdgurl

    cd $BUILDDIR

    mkdir -p $BUILDDIR/xdgurl.AppDir/usr/bin
    install -m 755 -p $BUILDDIR/$PROJNAME/xdgurl $BUILDDIR/xdgurl.AppDir/xdgurl
    install -m 644 -p $BUILDDIR/$PROJNAME/src/desktop/xdgurl.desktop $BUILDDIR/xdgurl.AppDir/xdgurl.desktop
    install -m 644 -p $BUILDDIR/$PROJNAME/src/desktop/xdgurl.svg $BUILDDIR/xdgurl.AppDir/xdgurl.svg
    install -m 755 -p /usr/bin/update-desktop-database $BUILDDIR/xdgurl.AppDir/usr/bin/update-desktop-database
    install -m 755 -p /usr/bin/desktop-file-validate $BUILDDIR/xdgurl.AppDir/usr/bin/desktop-file-validate
    install -m 755 -p /usr/bin/desktop-file-install $BUILDDIR/xdgurl.AppDir/usr/bin/desktop-file-install

    linuxdeployqt.appimage $BUILDDIR/xdgurl.AppDir/xdgurl -qmldir=$BUILDDIR/$PROJNAME/src/app/qml -verbose=2 -bundle-non-qt-libs # https://github.com/probonopd/linuxdeployqt/issues/25
    linuxdeployqt.appimage $BUILDDIR/xdgurl.AppDir/xdgurl -qmldir=$BUILDDIR/$PROJNAME/src/app/qml -verbose=2 -bundle-non-qt-libs # twice because of #25

    rm $BUILDDIR/xdgurl.AppDir/AppRun
    install -m 755 -p $BUILDDIR/$PROJNAME/src/desktop/appimage-desktopintegration $BUILDDIR/xdgurl.AppDir/AppRun

    linuxdeployqt.appimage --appimage-extract
    ./squashfs-root/usr/bin/appimagetool $BUILDDIR/xdgurl.AppDir
}

if [ $1 ] && [ $1 = 'ubuntu' ]; then
    build_ubuntu
elif [ $1 ] && [ $1 = 'fedora' ]; then
    build_fedora
elif [ $1 ] && [ $1 = 'arch' ]; then
    build_arch
elif [ $1 ] && [ $1 = 'appimage' ]; then
    build_appimage
else
    echo 'sh build.sh [ubuntu|fedora|arch|appimage]'
fi

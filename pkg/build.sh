#!/bin/sh

TMPDIR=$HOME/tmp
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

if [ $1 ] && [ $1 = 'ubuntu' ]; then
    build_ubuntu
elif [ $1 ] && [ $1 = 'fedora' ]; then
    build_fedora
elif [ $1 ] && [ $1 = 'arch' ]; then
    build_arch
else
    echo 'sh build.sh [ubuntu|fedora|arch]'
fi

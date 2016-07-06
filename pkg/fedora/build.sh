#!/bin/sh

cd `dirname $0`

mkdir -p ~/rpmbuild/BUILD
mkdir -p ~/rpmbuild/SPECS
cp -r ../../../xdgurl ~/rpmbuild/BUILD/
cp xdgurl.spec ~/rpmbuild/SPECS/
rpmbuild -bb ~/rpmbuild/SPECS/xdgurl.spec

#!/bin/sh

cd `dirname $0`

debuild -uc -us -b
#fakeroot debian/rules clean

#!/bin/sh

cd `dirname $0`

tar -czvf /tmp/xdgurl.tar.gz ../../../xdgurl
mv /tmp/xdgurl.tar.gz ./
updpkgsums
makepkg -s

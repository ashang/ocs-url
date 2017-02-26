#!/bin/bash

PROJDIR="$(cd "$(dirname "${0}")/../" && pwd)"

if [ ! -d "${PROJDIR}/lib/qtlib" ]; then
    #curl -L -o qtlib-master.tar.gz https://github.com/akiraohgaki/qtlib/archive/master.tar.gz
    #tar -xzvf qtlib-master.tar.gz
    #mv qtlib-master "${PROJDIR}/lib/qtlib"
    #rm qtlib-master.tar.gz
    git clone https://github.com/akiraohgaki/qtlib.git "${PROJDIR}/lib/qtlib"
fi

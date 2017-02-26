#!/bin/bash

PROJDIR="$(cd "$(dirname "${0}")/../" && pwd)"

if [ ! -d "${PROJDIR}/lib/qtlib" ]; then
    git clone https://github.com/akiraohgaki/qtlib.git -b master --single-branch --depth=1 "${PROJDIR}/lib/qtlib"
fi

#!/bin/bash

################################################################################
# This is utility script to import build dependencies
################################################################################

PROJDIR="$(cd "$(dirname "${0}")/../" && pwd)"

if [ ! -d "${PROJDIR}/lib/qtlib" ]; then
    git clone https://github.com/akiraohgaki/qtlib.git -b release-0.2.0 --single-branch --depth=1 "${PROJDIR}/lib/qtlib"
fi

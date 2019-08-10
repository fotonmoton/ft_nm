#!/bin/bash

PWD=$(pwd)
OUT_DIR="$PWD/out"
RELEASE_BLD_DIR="$PWD/bld"
DEBUG_BLD_DIR="$PWD/debug_bld"

FT_NM="ft_nm"
FT_OTOOL="ft_otool"

meson $RELEASE_BLD_DIR --prefix=$OUT_DIR --libdir=$OUT_DIR --includedir=$OUT_DIR --bindir=$OUT_DIR --buildtype=release
meson $DEBUG_BLD_DIR --buildtype=debug

ninja -C $DEBUG_BLD_DIR
ninja -C $RELEASE_BLD_DIR
ninja -C $RELEASE_BLD_DIR install

ln -sf "$OUT_DIR//$FT_NM" "$PWD/$FT_NM"
ln -sf "$OUT_DIR/$FT_OTOOL" "$PWD/$FT_OTOOL"

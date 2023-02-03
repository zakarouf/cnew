#!/bin/sh

PROJECT_NAME="cnew"
CC=gcc
CCFLAGS="-Wall -Wextra -lzkcollection"
TESTFLAGS=""
AR=ar
ARFLAGS="-q"
OUT_BIN="cnew"
OUT_LIB="libcnew.a"
OUT_HEAD="cnew"
BIN_DIR="/usr/local/bin/"
LIB_DIR="/usr/local/lib/"
INC_DIR="/usr/local/include/"

check_build() {
    if [ -d "./build" ]; then
        continue
    else
        $(mkdir ./build)
    fi
}

make_clean() {
    clean_what="./build"
    casevar=$1
    case "$casevar" in
        "lib") clean_what="$clean_what/lib"
        ;;
        "headers") clean_what="$clean_what/include"
        ;;
        "test") clean_what="$clean_what/test"
        ;;
        "bin") clean_what="$clean_what/bin"
        ;;
        "ofiles") clean_what="*.o"
        ;;
        "all") clean_what="$clean_what *.o"
        ;;
        *) echo "Unknown option '$*'"
           return
        ;;
    esac

    $(rm -rf $clean_what)
}

make_lib_header() {
    name=$OUT_HEAD
    $(find "./src/lib" -name '*.h' | cpio -pdm ./build/include)
    $(rm -rf ./build/$name)
    $(mv ./build/include/src/lib ./build/include/$name)
    echo "Made headers as $name in ./build/include"
}

make_lib(){
    extraflags="$1"
    FILES=$(find "./src/lib" -name '*.c')
    $($CC $CCFLAGS $extraflags -c $FILES)
    $($AR $ARFLAGS $OUT_LIB *.o)
    make_clean "all"
    $(mkdir ./build)
    $(mkdir ./build/lib)
    $(mv $OUT_LIB ./build/lib)
}

make_bin(){
    make_lib
    FILES=$(find "./src/bin" -name '*.c')
    $(mkdir ./build/bin)
    $($CC $CCFLAGS $FILES ./build/lib/$OUT_LIB -o ./build/bin/$OUT_BIN)
}

make_install_lib() {
    make_lib
    $(mv -ni ./build/lib/$OUT_LIB "$LIB_DIR")
    $(cp -Rf ./build/include/$OUT_HEAD "$INC_DIR")
    $(rm -rf ./build/include/$OUT_HEAD)
}

make_install_bin() {
    make_bin
    $(mv -ni ./build/bin/$OUT_BIN "$BIN_DIR")
}

make_install() {
    case "$1" in
        "bin") make_install_bin
        ;;
        "lib") make_install_lib
        ;;
        *) echo "Unknown option '$1', either use 'bin' or 'lib'"
        ;;
    esac
}

make_test(){
    make_lib $TESTFLAGS
    FILES=$(find "./src/test" -name '*.c')
    $($CC $CCFLAGS $FILES ./build/lib/$OUT_LIB -o "./build/test")
}

make() {
    check_build $1
    case "$1" in
        "lib") make_lib
        ;;
        "headers") make_lib_header
        ;;
        "bin") make_bin
        ;;
        "install") make_install $2
        ;;
        "test") make_test
        ;;
        "clean") 
            make_clean $2
        ;;
        *)
            echo "Unknown option '$1'"
            echo "Usage: $0 [ lib | headers | bin | test | install[lib|bin]]"
        ;;
    esac

}

make $*

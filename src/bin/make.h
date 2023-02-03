#define TEMPLATE_FMT \
"#!/bin/sh\n"\
"\n"\
\
"PROJECT_NAME=\"%s\"\n"\
\
"CC=%s\n"\
"CCFLAGS=\"%s\"\n"\
"LIBFLAGS=\"%s\"\n"\
"TESTFLAGS=\"%s\"\n"\
\
"AR=%s\n"\
"ARFLAGS=\"%s\"\n"\
\
"OUT_BIN=\"%s\"\n"\
"OUT_LIB=\"lib%s.a\"\n"\
"OUT_HEAD=\"%s\"\n"\
\
"BIN_DIR=\"%s\"\n"\
"LIB_DIR=\"%s\"\n"\
"INC_DIR=\"%s\"\n"\
"\n"\
"check_build() {\n"\
"    if [ -d \"./build\" ]; then\n"\
"        continue\n"\
"    else\n"\
"        $(mkdir ./build)\n"\
"    fi\n"\
"}\n"\
"\n"\
"make_clean() {\n"\
"    clean_what=\"./build\"\n"\
"    casevar=$1\n"\
"    case \"$casevar\" in\n"\
"        \"lib\") clean_what=\"$clean_what/lib\"\n"\
"        ;;\n"\
"        \"headers\") clean_what=\"$clean_what/include\"\n"\
"        ;;\n"\
"        \"test\") clean_what=\"$clean_what/test\"\n"\
"        ;;\n"\
"        \"bin\") clean_what=\"$clean_what/bin\"\n"\
"        ;;\n"\
"        \"ofiles\") clean_what=\"*.o\"\n"\
"        ;;\n"\
"        \"all\") clean_what=\"$clean_what *.o\"\n"\
"        ;;\n"\
"        *) echo \"Unknown option '$*'\"\n"\
"           return\n"\
"        ;;\n"\
"    esac\n"\
"\n"\
"    $(rm -rf $clean_what)\n"\
"}\n"\
"\n"\
"make_lib_header() {\n"\
"    name=$OUT_HEAD\n"\
"    $(find \"./src/lib\" -name '*.h' | cpio -pdm ./build/include)\n"\
"    $(rm -rf ./build/$name)\n"\
"    $(mv ./build/include/src/lib ./build/include/$name)\n"\
"    echo \"Made headers as $name in ./build/include\"\n"\
"}\n"\
"\n"\
"make_lib(){\n"\
"    extraflags=\"$1\"\n"\
"    FILES=$(find \"./src/lib\" -name '*.c')\n"\
"    $($CC $CCFLAGS $extraflags -c $FILES)\n"\
"    $($AR $ARFLAGS $OUT_LIB *.o)\n"\
"    make_clean \"all\"\n"\
"    $(mkdir ./build)\n"\
"    $(mkdir ./build/lib)\n"\
"    $(mv $OUT_LIB ./build/lib)\n"\
"}\n"\
"\n"\
"make_bin(){\n"\
"    make_lib\n"\
"    FILES=$(find \"./src/bin\" -name '*.c')\n"\
"    $(mkdir ./build/bin)\n"\
"    $($CC $CCFLAGS $LIBFLAGS $FILES ./build/lib/$OUT_LIB -o ./build/bin/$OUT_BIN)\n"\
"}\n"\
"\n"\
"make_install_lib() {\n"\
"    make_lib\n"\
"    $(mv -ni ./build/lib/$OUT_LIB \"$LIB_DIR\")\n"\
"    $(cp -Rf ./build/include/$OUT_HEAD \"$INC_DIR\")\n"\
"    $(rm -rf ./build/include/$OUT_HEAD)\n"\
"}\n"\
"\n"\
"make_install_bin() {\n"\
"    make_bin\n"\
"    $(mv -ni ./build/bin/$OUT_BIN \"$BIN_DIR\")\n"\
"}\n"\
"\n"\
"make_install() {\n"\
"    case \"$1\" in\n"\
"        \"bin\") make_install_bin\n"\
"        ;;\n"\
"        \"lib\") make_install_lib\n"\
"        ;;\n"\
"        *) echo \"Unknown option '$1', either use 'bin' or 'lib'\"\n"\
"        ;;\n"\
"    esac\n"\
"}\n"\
"\n"\
"make_test(){\n"\
"    make_lib $TESTFLAGS\n"\
"    FILES=$(find \"./src/test\" -name '*.c')\n"\
"    $($CC $CCFLAGS $FILES ./build/lib/$OUT_LIB -o \"./build/test\")\n"\
"}\n"\
"\n"\
"make() {\n"\
"    check_build $1\n"\
"    case \"$1\" in\n"\
"        \"lib\") make_lib\n"\
"        ;;\n"\
"        \"headers\") make_lib_header\n"\
"        ;;\n"\
"        \"bin\") make_bin\n"\
"        ;;\n"\
"        \"install\") make_install $2\n"\
"        ;;\n"\
"        \"test\") make_test\n"\
"        ;;\n"\
"        \"clean\") \n"\
"            make_clean $2\n"\
"        ;;\n"\
"        *)\n"\
"            echo \"Unknown option '$1'\"\n"\
"            echo \"Usage: $0 [ lib | headers | bin | test | install[lib|bin]]\"\n"\
"        ;;\n"\
"    esac\n"\
"\n"\
"}\n"\
"\n"\
"make $*\n"


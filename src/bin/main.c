#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <z_/types/string.h>
#include <z_/imp/subp.h>
#include <z_/imp/termio.h>
#include <z_/imp/argparse.h>
#include <z_/imp/sys.h>

#include "make.h"

typedef struct {
    z__String
      project_name
    , cc
    , cflags
    , libflags
    , testflags
    , ar
    , arflags
    , out_bin
    , out_lib
    , out_head
    , install_path_bin
    , install_path_lib
    , install_path_inc
    , make
    ;
} makeConfig;

z__String generate_make(makeConfig cfg) {
    char const *temp = TEMPLATE_FMT;
    if(cfg.make.lenUsed > 0) temp = cfg.make.data;

    z__String make = z__String_newFrom(
        temp,

        cfg.project_name.data,

        cfg.cc.data,
        cfg.cflags.data,
        cfg.libflags.data,
        cfg.testflags.data,

        cfg.ar.data,
        cfg.arflags.data,

        cfg.out_bin.data,
        cfg.out_lib.data,
        cfg.out_head.data,

        cfg.install_path_bin.data,
        cfg.install_path_lib.data,
        cfg.install_path_inc.data
    );

    return make;
}

#define FILE_TEMP_BIN_MAIN \
    "#include <stdio.h>\n"\
    "#include <string.h>\n"\
    "#include <stdlib.h>\n"\
    "\n"\
    "int main (int argc, char const *argv[])\n"\
    "{\n"\
    "\n"\
    "    return 0;\n"\
    "}\n"

int cnew(char *const name, makeConfig conf)
{
    z__String str = z__String_new(128);
    #define rp(fmt, ...) z__String_replace(&str, fmt,##__VA_ARGS__)
    #define run(fmt, ...) z__String_replace(&str, fmt,##__VA_ARGS__); z__subp_spawn(str.data)

    run("git init %s", name);
    run("mkdir %s/src", name);
    run("mkdir %s/src/lib", name);
    run("mkdir %s/src/bin", name);
    run("mkdir %s/src/test", name);
    run("mkdir %s/docs", name);
    run("mkdir %s/examples", name);

    z__String_replace(&str, "lib%s.a", name);
    z__String make = generate_make(conf);

    rp("%s/make", name);

    FILE *fp = fopen(str.data, "w");
    if(fp == NULL) return -1;
    fwrite(make.data, make.lenUsed, sizeof(*make.data), fp);
    fclose(fp);

    rp("%s/src/bin/main.c", name);
    fp = fopen(str.data, "w");
    if(fp == NULL) z__panic(-1, "Could not open %s, to write template\n", str.data);
    fwrite(FILE_TEMP_BIN_MAIN, sizeof(FILE_TEMP_BIN_MAIN)-1, 1, fp);
    fclose(fp);

    rp("%s/README.md", name);
    fp = fopen(str.data, "w");
    if(fp == NULL) z__panic(-1, "Could not open %s, to write template\n", str.data);
    rp("# %s\n\n", name);
    fwrite(str.data, str.lenUsed, 1, fp);
    fclose(fp);

    z__String_delete(&make);
    z__String_delete(&str);
    return 0;
}

makeConfig argparse(int argc, char const *argv[])
{
    #define x(x) (x, sizeof(x)-1)
    makeConfig conf = {
          .cc = z__String_newFromStr("gcc", 3)
        , .testflags = z__String_new(16)
        , .project_name = z__String_new(16)
        , .arflags = z__String_newFrom("-q")
        , .ar = z__String_newFrom("ar")
        , .cflags = z__String_newFrom("-Wall -Wextra")
        , .libflags = z__String_new(16)
        , .out_bin = z__String_new(16)
        , .out_lib = z__String_new(16)
        , .out_head = z__String_new(16)
        , .install_path_bin = z__String_newFrom("/usr/local/bin/")
        , .install_path_lib = z__String_newFrom("/usr/local/lib/")
        , .install_path_inc = z__String_newFrom("/usr/local/include/")
        , .make = {0}
    };
    #define set(what) \
        z__argp_next();\
        z__String_replace(&conf.what, "%s", z__argp_get())

    #define arg(str, what) z__argp_elifarg_custom(x(str)) { set(what); }

    #define HELP_TXT \
    "cnew: A C Utility Library\n"\
    "Usage: cnew [project_name] OPTIONS\n\n"\
    "  --help       Show Help Text\n"\
    "  --cc         Set C Compiler\n"\
    "  --cflags     Set C Flags\n"\
    "  --libflags   Set C Library Flags\n"\
    "  --ar         Set Archiver\n"\
    "  --obin       Set Out Binary Executable Name\n"\
    "  --olib       Set Out Library Name, default is based on project name\n"\
    "  --ohead      Set Out Header Folder Name, default is project name\n"\
    "  --path_bin   Set Path for Binary Installation\n"\
    "  --path_lib   Set Path for Library Installation\n"\
    "  --path_inc   Set Path for Header Installation\n"\
    "  --testflags  Set C Flags for tests\n"\

    z__argp_start(argv, 1, argc) {
        z__argp_ifarg_custom(x("--help")) { puts(HELP_TXT); exit(0); }
        arg("--testflags", testflags)
        arg("--ar", ar)
        arg("--arflags", arflags)
        arg("--cflags", cflags)
        arg("--libflags", libflags)
        arg("--obin", out_bin)
        arg("--olib", out_lib)
        arg("--ohead", out_head)
        arg("--path_bin", install_path_bin)
        arg("--path_lib", install_path_lib)
        arg("--path_inc", install_path_inc)
        z__argp_elifarg_custom(("--temp", 6)) {
            z__argp_next();
            char const *name = z__argp_get();
            conf.make = z__String_newFromFile(name);
        }
        else {
            if(conf.project_name.lenUsed == 0) {
                z__String_replace(&conf.project_name, "%s", z__argp_get());
            }
            printf("%s\n", conf.project_name.data);
        }
    }

    if(conf.project_name.lenUsed == 0) z__panic(-1, "Project name not specified");
    if(conf.out_bin.lenUsed == 0)  z__String_replaceStr(&conf.out_bin, conf.project_name.data, conf.project_name.lenUsed);
    if(conf.out_lib.lenUsed == 0)  z__String_replace(&conf.out_lib, "%s", conf.project_name.data);
    if(conf.out_head.lenUsed == 0) z__String_replaceStr(&conf.out_head, conf.project_name.data, conf.project_name.lenUsed);

    return conf;
}

void make_make(char const *fname)
{
    z__String str = z__String_newFromFile(fname);
    z__String str_out = z__String_new(str.lenUsed + 1024);
    z__String_append(&str_out, 0, ' ', "#define TEMPLATE_FMT \\\n\"");
    for (size_t i = 0; i < str.lenUsed; i++) {
        if(str.data[i] == '\n') {
            z__String_append(&str_out, 0, ' ', "\\n\"\\\n\"");
        } else if (str.data[i] == '"') {
            z__String_pushChar(&str_out, '\\');
            z__String_pushChar(&str_out, '"');
        } else {
            z__String_pushChar(&str_out, str.data[i]);
        }
    }
    
    z__termio_putString(&str_out);
    putc('\n', stdout);
}

int main(int argc, char const *argv[])
{
    if(argc >= 2) {
        cnew((char *)argv[1], argparse(argc, argv));
    } else {
        printf("Usage: %s [project_name] OPTIONS\n"
               "Use --help to view all `OPTIONS`\n", argv[0]);
    }
    return 0;
}

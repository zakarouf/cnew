# cnew

C Utility Application

* Initialization of a new project
* Auto generation of shell script to build project

## Installation
```
$ cd cnew
$ ./build.sh install bin
```

## Usage
```
$ cnew myproject
```

## Commands
```sh
$ cnew --help
cnew: A C Utility Library
Usage: cnew [project_name] OPTIONS

  --help       Show Help Text
  --cc         Set C Compiler
  --cflags     Set C Flags
  --libflags   Set C Library Flags
  --ar         Set Archiver
  --obin       Set Out Binary Executable Name
  --olib       Set Out Library Name, default is based on project name
  --ohead      Set Out Header Folder Name, default is project name
  --path_bin   Set Path for Binary Installation
  --path_lib   Set Path for Library Installation
  --path_inc   Set Path for Header Installation
  --testflags  Set C Flags for tests
```

## Structure
Initialization a project with `cnew` will create a new directory with following structure.
```python
.
├── build.sh        # Build Script
├── docs            # Documentation
├── examples        # Examples
├── README.md       # Readme
└── src             # Source Code Folder
   ├── bin          # Code that will be used for creating an executable
   │  └── main.c
   ├── lib          # Cdoe that will be used for creating a library
   └── test         # Code for Test Builds

```

The `build.sh` script will have all the prequisite code and config for creating building and Installation.

```sh
$ head -n 14 ./build.sh
#!/bin/sh

PROJECT_NAME="myproject"
CC=gcc
CCFLAGS="-Wall -Wextra"
LIBFLAGS=""
TESTFLAGS=""
AR=ar
ARFLAGS="-q"
OUT_BIN="myproject"
OUT_LIB="libmyproject.a"
OUT_HEAD="myproject"
BIN_DIR="/usr/local/bin/"
LIB_DIR="/usr/local/lib/"
```
Shell scripts are pretty easy to edit, and first 14 or so lines of this script dictates how the system works, so if you want to do some changes then it will be pretty easy.

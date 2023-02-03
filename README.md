# cnew

C Utility Application

* Initialization of a new project
* Auto generation of shell script to build project

## Installation
```
$ cd cnew
$ ./make install bin
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


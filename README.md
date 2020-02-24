# C++ Class Manager

> __Warning !__ This project is not currently in a stable version,
it was not tested / optimized for every case / configurations

cxxm is a command line tool for managing cmake built c++ projects. you can add, delete or move your source and header files and automatically manage your CMakeLists.txt sources with one command line.

## Installation

Build the cxxm project with cmake

## Usage

### Commands

#### init

```txt
cxxm init (<options>) [<project-name>]
```

Init a new project with a basic CMakeLists.txt in the current directory

__options__ :

- __--directory__ | __-d__ : init the project in a new created directory

- __--tests__ | __-t__ : init the project with a tests sub-project

#### add

```txt
cxxm add (<options>) [<paths>]
```

Create class header and source files and add the source to the CMakeLists.txt file
> Note that cxxm will automatically create directories that don't already exists

__options__ :

- __--header-only__ | __-h__ : only create the header file

- __--global__ | __-g__ : path is relative to the sources / includes folder (that's the default behavior)

- __--local__ | __-l__ : path is local to your current position. So you need to be in the sources or includes folder otherwise the command fail

#### move

```txt
cxxm move (<options>) [<path-from>] (<options>) [<path-to>]
```

Move class header and source files and rename the source in the CMakeLists.txt file. Also rename the include reference in the source and the header's definition name

> Note that cxxm will automatically create directories that don't already exists and delete empty directories after a file move

__options__ :

- __--global__ | __-g__ : path is relative to the sources / includes folder (that's the default behavior)

- __--local__ | __-l__ : path is local to your current position. So you need to be in the sources or includes folder otherwise the command fail

#### remove

```txt
cxxm remove (<options>) [<path>]
```

Remove class header and source files and remove the source from the CMakeLists.txt file

> Note that cxxm will automatically delete empty directories after a file remove

__options__ :

- __--global__ | __-g__ : path is relative to the sources / includes folder (that's the default behavior)

- __--local__ | __-l__ : path is local to your current position. So you need to be in the sources or includes folder otherwise the command fail

#### dump defaults

```txt
cxxm dump defaults
```

Dump default contents files in the \<user\>/.cxxm/files directory so you can overwrite the content used by cxxm to create files

### Basic CMakeLists.txt

```txt
cmake_minimum_required(VERSION 3.1)

project(MyProject)

include_directories(includes) <- needed for correct include

set(CXXM_SOURCES
#CXXM_BEGIN         <- needed for source management
sources/main.cpp)
#CXXM_END           <- needed for source management

add_executable(MyProject ${CXXM_SOURCES})
```

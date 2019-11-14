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
cxxm init
```

- Init a new project with a basic CMakeLists.txt in the current directory

#### add

```txt
cxxm add <options> <paths/to/new/class>
```

- Create new class (header and source)
- Create the non-existing directories of the path

#### move

```txt
cxxm move <options> <paths/to/class> <options> <new/paths/to/class>
```

- Move / Rename class (header, source and include reference)
- Remove the last header and source directory if empty
- Create the non-existing directories of the new path

#### remove

```txt
cxxm remove <options> <paths/to/class>
```

- Remove class (header and source)
- Remove the header and source directory if empty

### Options

#### --local / --global

Commands : __[add, remove, move]__

- __global :__ The following path is relative to the sources / includes folder (that's the default behaviour)

- __local :__ The following path is local to your current position.
So you need to be in the sources or includes folder otherwise the command fail

#### --header-only

Commands : __[add]__

- Only create the header of the class

### Basic CMakeLists.txt

```txt
cmake_minimum_required(VERSION 3.1)
project(my_project)

set(SOURCES
#CXXM_BEGIN   <- needed for source management
sources/main.cpp)
#CXXM_END     <- needed for source management

include_directories(includes) # <- needed for correct include

add_executable(my_project ${SOURCES})
```

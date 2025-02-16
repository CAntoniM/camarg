# camargs

a lib for parsing command line arguments in a pluggable and expandable mathod.

## Usage

camargs has a core object which is defined at `cam::arg::Parser` which is used
to define 3 types of command line arguments, `flags`, `args` and `subcommand`. args
are poistional arguments which are parsed in the oder they are added. `flags` which
are optional elements that turn on particular feature which are defined by the -
or --.

### Basic usage

When creating a basic argument parser you will need to call the constructor of
the object with 2 parameters a _name_ and a _help_ with the name being the name
of executable that is in use and help is the description of the programme.

```cpp
#include <camarg.h>

int main(int argc, const char* argv[]) {
    cam::arg::Parser parser(
        "ls", //name of the application
        "list the contents of a directory" //description of the app
    );
};
```

then you will define some arguments on them you would do this with the arg
method. this takes a `name`, `help`, `default_value` these define what name
is used to retreave the argument, the help the describes the argument, .

```cpp
#include <camarg.h>

int main(int argc,const char* argv[]){
    cam::arg::Parser parser("ls","list the contents of a directory");
    parser.arg(
        "dir", // this is the name of the argument that is parsed
        "where you want to search", // this is the description of the argument
        "." // this is the default value
    );
}
```

then you will parse the arguments using the `parse` method will taken the
command line arugments from the operating system. and get back your arguments
from the parser using the `get` module.

```cpp
#include <camarg.h>
#include <string>
int main(int argc, const char* argv[]) {
    cam::arg::Parser parser("ls","list the contents of a directory");
    parser.arg("dir","where you want to search", ".");

    parser.parse(argc,argv);

    std::string dir = parser.get("dir");
}
```

for a more advanced look at the different methods availble

# building the library

we use cmake for our build tool meaning that we need to run the following the
commands

```bash
mkdir build
```

this makes directory the git is configured to ignore

```bash
cd build; cmake ..
```

This will run the cmake command to in the directory generating out the default
build tool for your system this will be

```bash
cmake --build .
```

this will run the build tool and produce out the libcamarg.dll or libcamarg.so

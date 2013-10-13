# Preshell

[![Build Status](https://secure.travis-ci.org/sabel83/preshell.png?branch=master "Build Status")](http://travis-ci.org/sabel83/preshell)

Preshell is an interactive C/C++ preprocessor shell. It is intended to be used
for trying out non-trivial preprocessor usage, such us using the macros of
[Boost.Preprocessor](http://boost.org/libs/preprocessor).

The shell simulates a preprocessor. It receives the input line by line coming
from the shell. The result of preprocessing is immediately displayed to the
user.

The shell is based on the [Boost.Wave](http://boost.org/libs/wave) and the
[Readline](http://www.gnu.org/software/readline) libraries.

## Table of contents

- [Features](#features)
- [Building Preshell](#building-preshell)
- [Getting started](#getting-started)
    - [Evaluating simple expressions](#evaluating-simple-expressions)
    - [Checking which macros are available](#checking-which-macros-are-available)
    - [Detecting that a header file is included into a shell](#detecting-that-a-header-file-is-included-into-a-shell)
- [License](#license)

## Features

* History of commands used
* Tab-completion for preprocessor directives and macro names
* Differentiating error messages from normal output using colors
* Any preprocessing command can be interrupted using `Ctrl-C` (only on Linux)
* Debug pragmas:
    * `#pragma wave macro_names` displays the names of defined macros
    * `#pragma wave macros` display the defined macros and their definitions

## Building Preshell

* Download the source code from [github](http://github.com/sabel83/preshell).
* Install the dependent libraries:
    * Boost.Wave
    * Boost.Test
    * Boost.ProgramOptions
    * Readline
* Install CMake
* Set the right system include path and default macro definitions
    * This step is important for Preshell to find the same headers GCC or Clang
      uses
    * `tools/gcc_default_path > lib/default_sysinclude.hpp`
    * `tools/gcc_builtin_macros > lib/default_defines.hpp`
* In the source directory run the following commands:
    * `mkdir bin`
    * `cd bin`
    * `cmake ..`
    * `make`
    * To install it on the host run `make install`
    * To package it for Debian or Ubuntu run `cpack -G DEB`
        * Note: the dependencies are configured for Ubuntu 13.04. To package it
          for another system, check the dependency list. The Boost library
          version the package will depend on may be different. Update the value
          of `CPACK_DEBIAN_PACKAGE_DEPENDS` in `CMakeLists.txt`.
    * To package it for OpenSUSE or Fedora run `cpack -G RPM`

## Getting started

This section presents how to use Preshell to do simple debugging.

### Evaluating simple expressions

Let's look at how to evaluate the expression `6 + 7`. If you have access to
`python`, `ghci` or `erl`, start it and type `6 + 7` (`6 + 7.` in Erlang) and
press enter. You will see `13`, the result of this expression immediately.

Let's do the same in preprocessor metaprogramming. Start `preshell`.
Addition is implemented by the `BOOST_PP_ADD` macro - get it by running the
following command:

```cpp
> #include <boost/preprocessor/arithmetic/add.hpp>
```

You will see a number of empty lines printed on your screen. The reason behind
it is that Preshell displays the result of preprocessing the included file.
The preprocessor directives are processed (and produce no output) but all the
empty lines in the included file are displayed.

Once you have set your environment up, you can evaluate the expression `6 + 7`
by running the following:

```cpp
> BOOST_PP_ADD(6, 7)
```

You get `13`, which is the result of this addition. You can try adding other
numbers as well. Note that you can use the arrow keys to re-run earlier commands
or slightly updated versions of them. You can also try tab completion out by
typing `BOOST_PP` and pressing `tab`. Preshell will show you all macros starting
with `BOOST_PP`.

Note that you can add arbitrarily large numbers using `BOOST_PP_ADD`. The
arguments (and the result) have to be in the range `0 .. BOOST_PP_LIMIT_MAG`
where `BOOST_PP_LIMIT_MAG` is a macro. Let's find out its value. It is defined
in the following header:

```cpp
> #include <boost/preprocessor/config/limits.hpp>
```

The above command makes the `BOOST_PP_LIMIT_MAG` macro available, so it is easy
to check its value:

```cpp
> BOOST_PP_LIMIT_MAG
256
```

### Checking which macros are available

Preshell can be used to get the list of available macros. The following command
lists the names of the macros:

```cpp
> #pragma wave macro_names
```

The result of it is the list of macros defined at point where the pragma was
used. The following command displays the definition of the macros as well:

```cpp
> #pragma wave macros
```

The output of this command can be long, since it displays all macros and their
actual definition.

### Detecting that a header file is included into a shell

If you are a developer of a header file, you might be interested if it is used
from Preshell or not. Preshell defines the `__PRESHELL` macro by default which
you can check, but note that this macro may be undefined.

It is also possible to check the version of Preshell. The macros
`__PRESHELL_MAJOR`, `__PRESHELL_MINOR` and `__PRESHELL_PATCH` are provided for
that. Note that these macros can also be undefined or redefined.

## License

Preshell is published under the
[GNU General Public License, version 3](http://www.gnu.org/licenses/gpl.html).


Preshell
========

[![Build Status](https://secure.travis-ci.org/sabel83/preshell.png?branch=master "Build Status")](http://travis-ci.org/sabel83/preshell)

Prehsell is an interactive C/C++ preprocessor shell. It is intended to be used
for trying out non-trivial preprocessor usage, such us using the macros of
[Boost.Preprocessor](http://boost.org/libs/preprocessor).

The shell simulates an preprocessor. It receives the input line by line coming
from the shell. The result of preprocessing is immediately displayed to the
user.

The shell is based on the [Boost.Wave](http://boost.org/libs/wave) and the
[Editline](http://www.thrysoee.dk/editline/) libraries.

Features
--------

* History of commands used
* Tab-completion for preprocessor directives and macro names
* Differentiating error messages from normal output using colors
* Any preprocessing command can be interrupted using `Ctrl-C` (only on Linux)
* Debug pragmas:
    * `#pragma wave macro_names` displays the names of defined macros
    * `#pragma wave macros` display the defined macros and their definitions

Building Preshell
-----------------

* Download the source code from [github](http://github.com/sabel83/preshell).
* Install the dependent libraries:
    * Boost.Wave
    * Boost.Test
    * Boost.ProgramOptions
    * Editline
* Install CMake
* In the source directory run the following commands:
    * `mkdir bin`
    * `cd bin`
    * `cmake ..`
    * `make`
    * `make install`

License
-------

Preshell is published under the
[Boost Software License](http://www.boost.org/LICENSE_1_0.txt).


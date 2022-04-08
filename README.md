## Table of Contents
1. [General Info](#general-info)
2. [Technologies](#technologies)
3. [Installation](#installation)
4. [Usage](#usage)
### General Info
***
42sh is a Unix and Linux shell.
A project by Thomas Cacelles, Lucas Llombart, Nicolas Alastra and Dimitri Escaffre-Faure
## Technologies
***
List of technologies used:
* [Meson](https://mesonbuild.com/): Version 0.60.2
* [Criterion](https://criterion.readthedocs.io/en/master/setup.html): Version 2.3.3
## Installation
***
```
$ cd funny_leakey-42sh-2024
$ make 42sh
or
$ cd funny_leakey-42sh-2024
$ meson setup builddir
$ ninja -C builddir
```
## Usage
***
> script
$ ./builddir/42sh *.sh

> argument 
$ ./builddir/42sh -c "..."

> input
$ ./builddir/42sh
$ 42sh$ ...
$ 42sh$ exit -> to quit

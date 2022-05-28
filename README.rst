.. README.rst for coding-interview-prep

coding-interview-prep
=====================

Simple [#]_ code often written, referenced, or used during technical interviews.

Contains simple implementations of fundamental data structures, algorithms,
fixes of buggy code, etc. usually found in questions asked during software
engineering, quant, or technical research position interviews.

Since some questions are language dependent (ex. C++ references are very
different from Python references), the first level of subdirectories splits the
source by language, with the subsequent level specifying topics.

.. [#] To be fair, the implementations here are an order of magnitude cleaner
   than what is typically written under pressure in an interview. Unless asked,
   you wouldn't worry about inheritance and perfect-looking code.

Contents
--------

TBD

Building from source
--------------------

C/C++
~~~~~

.. _CMake: https://cmake.org/cmake/help/latest/

.. _Google Test: https://google.github.io/googletest/

.. _Ninja: https://ninja-build.org/

.. _Check: https://libcheck.github.io/check/

TBD. In short, build with CMake_ 3.16 or later.

For C, unit tests require the Check_ testing framework, while for C++, unit
tests require the `Google Test`_ testing framework. I chose to use two
different testing frameworks because usually, people don't compile their C
code with a C++ compiler.

On Windows, in order to correctly build the `Google Test`_ test runners, ensure
that when building `Google Test`_ with CMake_, that
``-Dgtest_force_shared_crt=1`` is passed to the ``cmake`` build configuration
command. Doing so forces `Google Test`_ to link against the shared C runtime
instead of linking against the static C runtime as it does by default.
Furthermore, ensure that the correct debug or release build is being linked
against.

C++ ABI compatibility
^^^^^^^^^^^^^^^^^^^^^

The C++ code in this repository contains classes that have STL members and
[may] also have functions/methods that return STL members. On Windows
especially, exporting STL objects to/from DLLs gives a lot of headaches due to
issues of ABI incompatibility, which are often specific to MSVC. See the
`detailed discussion on StackOverflow`__.

.. __: https://stackoverflow.com/a/22797419/14227825

Fortunately, since this repo is only distributed as source code, it is highly
unlikely that ABI issues will arise. If the user compiles this repo's C++ code
and links against it with their own code, for whatever reason, it is likely
the same compiler toolchain is being used, although care may have to be taken
when using MSVC to ensure that the proper debug/release DLL is linked against
depending on whether the user's code is for debug/release.


Python
~~~~~~

.. _setuptools: https://setuptools.pypa.io/en/latest/index.html

TBD. Pure Python source only requires Python 3.6 or later and setuptools_.

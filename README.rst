.. README.rst for pdcip

pdcip
=====

.. _Google Test: https://google.github.io/googletest/

.. _setuptools: https://setuptools.pypa.io/en/latest/index.html

.. _unittest: https://docs.python.org/3/library/unittest.html

.. _CMake: https://cmake.org/cmake/help/latest/

.. _Ninja: https://ninja-build.org/

.. _Check: https://libcheck.github.io/check/

Simple multi-language implementations of common data structures and algorithms.

Mostly a way for me to learn or refresh my knowledge of different programming
languages, their corresponding build systems, and exercise some design thinking
when implementing things in different languages. Source code is separated
by language [#]_ into different subprojects located in the appropriately named
top-level directories.

Note that originally, this repo was named ``coding-interview-prep``, with
``pdcip`` chosen as the namespace and library name in each language
implementation, an acronym for "PhetDam Coding Interview Prep". Now it has no
real meaning anymore, although an alternate interpretation for ``cip`` could be
"Coding Implementation Practice".

.. Contains simple implementations of fundamental data structures, algorithms,
   fixes of buggy code, etc. usually found in questions asked during software
   engineering, quant, or technical research position interviews.

.. [#] Separation is not perfect. For example, `Google Test`_, a C++ unit
   testing framework, is used for testing the C implementation, so of course
   the unit tests for the C implementation are written in C++.

Features
--------

.. _BFS: https://en.wikipedia.org/wiki/Breadth-first_search

.. _DFS: https://en.wikipedia.org/wiki/Depth-first_search

The following table shows current subproject languages, build system support,
and testing frameworks used.

+----------+------------------+---------------------+
| Language | Build System     | Testing Framework   |
+==========+==================+=====================+
| C        | CMake_           | `Google Test`_      |
+----------+------------------+---------------------+
| C++      | CMake_           | `Google Test`_      |
+----------+------------------+---------------------+
| Python   | setuptools_ [#]_ | unittest_           |
+----------+------------------+---------------------+

The next table shows which data structures/algorithms have been implemented
and in what languages. Languages for which implementation is incomplete are
marked with an asterisk ``*``.

+--------------------------+-------------------+
| Item                     | Languages         |
+==========================+===================+
| BFS_ (tree)              | C++, Python       |
+--------------------------+-------------------+
| DFS_ (tree)              | C, C++, Python    |
+--------------------------+-------------------+
| binary tree              | C++, Python       |
+--------------------------+-------------------+
| graph                    | Python            |
+--------------------------+-------------------+
| linked list              | C*, C++*          |
+--------------------------+-------------------+
| linked list (``void *``) | C*                |
+--------------------------+-------------------+
| tree                     | C, C++, Python    |
+--------------------------+-------------------+

.. [#] In the past, setuptools_ was the de-facto default Python build system.

Building from source
--------------------

   Note:

   In the future, each subproject directory will get its own ``README.rst``
   including build instructions.

C/C++
~~~~~

TBD. In short, build with CMake_ 3.16 or later.

Building C/C++ unit tests requires `Google Test`_. The C library can be called
from C++ code, as the C headers contain

.. code:: c

   #ifdef __cplusplus
   extern "C" {
   #endif /* __cplusplus */

   /* ... */

   #ifdef __cplusplus
   }
   #endif /* __cplusplus */

On Windows, in order to correctly build the `Google Test`_ test runners, ensure
that when building `Google Test`_ with CMake_, that
``-Dgtest_force_shared_crt=1`` is passed to the ``cmake`` build configuration
command. Doing so forces `Google Test`_ to link against the shared C runtime
instead of linking against the static C runtime as it does by default.
Furthermore, ensure that the correct debug or release build is being linked
against.

There are also some legacy C unit tests written using the Check_ unit testing
framework which can be built if Check_ is installed and
``-DBUILD_CHECK_TESTS=1`` is passed to the ``cmake`` build configuration
command.

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

TBD. Pure Python source only requires Python 3.6 or later and setuptools_.

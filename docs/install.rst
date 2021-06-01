Requirements and Installation
=============================

Prerequisites
-------------

hicmaptools compilation requires the following tools installed on your
system ``make``, ``gcc-c++`` and ``R``.

Compile/Installation
--------------------

Clone the git repository on your computer with the following command:

::

    git clone git@github.com:cbcrg/hicmaptools.git hicmaptools

Make sure you have installed the required dependencies listed above.
When done, move in the project root folder named ``hicmaptools`` and
enter the following commands:

::

    $ cd src
    $ make

The binary will be automatically copied to the path ``hicmaptools/bin``.

::

    $ make install

The binary will be automatically copied to the path specified by the
environment variable ``$USER_BIN`` (check that it exists before run the
make command).

Requirements and Installation
=============================

Prerequisites
-------------

hicmaptools compilation requires the following tools installed on your system ``make``, ``gcc-c++`` and ``R``.

Compile/Installation
--------------------

Clone the git repository on your computer use below command:

::

    git clone git@github.com:changlabtw/hicmaptools.git hicmaptools

Move in the project root folder named ``hicmaptools`` and enter following commands. Then the binary will be automatically copied to the path ``./bin``

::

    $ cd src
    $ make

The below command will copy the excutable binary file ``hicmaptools`` to the path specified by the environment variable ``$USER_BIN`` (check that it exists before run the
make command)

::

    $ make install

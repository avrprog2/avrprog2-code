# AVRprog2 -- A Linux tool for the [MikroElektronia](http://www.mikroe.com) AVRprog2 programming hardware.

## Compile and Install

The following libraries are needed to compile this project:

- usb-1.0
- bfd (part of binutils)
- boost (especially boost\_filesystem and boost\_property\_tree)

Doxygen and Graphviz are needed for building the documentation and manpage.

compile and install with:

~~~~~~~~~~~~~~~~~~~~
./configure
make
sudo make install
~~~~~~~~~~~~~~~~~~~~

Another important make traget is
`make doc`

This will generate a html documentation of the source.

The application is configurable within [avrprog.h](avrprog.h):

- WRITE\_FUSES\_SUPPORT: turn on/off writing of fuse bytes
- CONFIG\_DIR: system wide configuration directory
- HOME\_CONFIG\_DIR: user specific configuration directory

## Usage

If you want to know how to use this tool, look into [man.txt](@ref man.txt).

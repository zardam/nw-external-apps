# Epsilon

The base firmware image is a fork of https://github.com/numworks/epsilon including
the "external" application and an API for accessing base functionality of epsilon.

The modified sources can be found here : https://github.com/Delta-NumWorks/delta/tree/external

# If something goes wrong

Do an official firmware upgrade. Follow the instructions on https://workshop.numworks.com/devices/upgrade.
You can always restore the calculator using this method.

I'm not affiliated with NumWorks. If your calculator is buggy when using this firmware, please restore the official one.

# Included applications

## KhiCAS

The port included here is a port from the delta firmware : https://github.com/Delta-NumWorks/delta/tree/delta

## Periodic

This is a port of https://bitbucket.org/m4x1m3/nw-atom

## Nofrendo

This is a port of Nofrendo from https://www.baisoku.org/

# Developer info

## Technical concepts

- The firmware part is just a regular application called "External", looking for a tar archive at address 0x90200000. If found, each file is displayed in a menu.
- To launch a file, the external application fetches the firsts 4 bytes of the file, and use it as a function pointer to the address of the entrypoint. The entrypoint is then called with an array of function pointers as en API, and a pointer to a pre-allocated heap.
- Each application is totally independent (and needs to bring it's own libc, ...). data and bss sections are allocated in ITCM RAM, and the heap is pre-allocated from epsilon. The state of the application is totally reset upon entry.
- To build a usable tar archive, relocation needs to be done to account the real adresses in flash of the binaries. There is a script called "archive" in this repository, which calls the linker on partially linked elf, to build final correctly relocated elf files (then added to the tar archive).
- The file in the tar must be marked as executable to be executable.

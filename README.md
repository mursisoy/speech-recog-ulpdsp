# Speech Recognition on C55xx ULP DSP Family

This system is an approach of a basic speech recognition processing system.

It was designed for Microprocessors Architecture subject in Telecomunications Engineering Grade at Unizar.

## Code Composer v6 Project Settings
For building and debugging we have to take care for some configurations as is detailed afterwards:

In Project Properties:

* Linker Command File: lnkx.cmd
* Runtime Library Support: rts55h.lib

Build > C5500 Compiler:

* Set ptrdiff_size=32

Build > C5500 Compiler > Processor Options:

* Set memory_model = huge

Build > C5500 Compiler > Include Options:

__Add dsplib include file path__

Build > C5500 Linker > File Search Path

__Add dsplib huge type .lib, we include this one 5xdsph_r3.lib__

# Speech Recognition on C55xx ULP DSP Family

This project implements a speaker recognition alforithm we designed based on Mel filterbank analysis.

The system was built for our Microprocessor Architecture subject in our Telecomunications Engineering degree at Universidad de Zaragoza.

## Code Composer v6 Project Settings
Before building and debugging we have to take care of some configurations:
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

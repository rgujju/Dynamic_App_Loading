### STM32 Dynamic Linking

<br />
<p align="center">
  <p align="center">
    Minimal example of dynamic apps on the STM32. Should be portable to any mcu, atleast the ARMv7-M
    <br />
    <a href="https://rgujju.github.io/STM32_Dynamic_Apps/html/index.html"><strong>Explore the docs »</strong></a>
    <br />
  </p>
</p>

## About The Project


## Usage
This example works on the STM32F429 DISC0 board. But should be portable to any other mcu.  
* First build the userlib.a. This is a static library aka archive. The app will be linked to this archive.  
``mkdir -p build/userlib``  
``cd build/userlib``  
``cmake ../.. -DTARGET_GROUP=userlib -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cross.cmake``  
``make``  
A folder called **lib** will be created. This folder will contain everything required to build the app.  
It contains the header files and the userlib.a archive.   
Copy this folder to the apps folder.  
``cp -R lib ../../apps/``  

* Now build the app. A example app called blinky is provided.  
This app turn on LED1. There is some extra code in the example to purposely populate the data and bss sections  
And also to verify if the GOT is copied properly. ie global variable access.  
``cd apps``  
``mkdir build/``  
``cd build``  
``cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cross.cmake``  
``make``  
This should have created **blinky.elf** file. This file now needs to be converted into TINF.

* Create TINF of the app
``python3 ../../elf2tinf/elf2tinf.py --major 1 --minor 0 blinky.elf blinky``  
This should generate 2 files, the **blinky.tinf** and **blinky_tinf.h**   
**blink.tinf** is in a binary format which can be loaded over uart, ble, usb, etc.  
**blinky_tinf.h** is the same binary data in the form of a header file, so I can test it easily, 
without implementing the actual transfer of the binary to the mcu.  
More details about this tool is in the README.md in folder elf2tinf  

* Build the kernel
``mkdir -p build/debug``  
``cd build/debug``  
Build the kernel, this is the code that will actually load the app and run it  
``cmake ../.. -DTARGET_GROUP=kernel -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cross.cmake``  
``make``  
This will generate the **main.elf** file which needs to be loaded onto the board.

* Load main.elf
This can vary depending on your method.  
If you use gdb then simply, with something like openocd already running in the background then simply  
``make flash``  

## Documentation
User documentation is in header files  
To go deeper see the source files  
Online docs: https://rgujju.github.io/STM32_Dynamic_Apps/html/index.html
## Acknowledgements
I learnt these things from the following resources:  
* https://github.com/bogdanm/udynlink
* https://github.com/tock/tock
* https://github.com/pebble-dev/RebbleOS/blob/master/rcore/appmanager.c
* https://www.airs.com/blog/archives/38
    - 40 part explaination about linkers
* https://www.technovelty.org/linux/plt-and-got-the-key-to-code-sharing-and-dynamic-libraries.html
    - Explains PLT and GOT
* https://stackoverflow.com/questions/3322911/what-do-linkers-do/33690144#33690144
    - Explains relocation with a proper worked out example
* https://stackoverflow.com/questions/2463150/what-is-the-fpie-option-for-position-independent-executables-in-gcc-and-ld/51308031#51308031
    - Explains PIE relocation with a proper worked out example
* https://stac47.github.io/c/relocation/elf/tutorial/2018/03/01/understanding-relocation-elf.html
    - worked out example of relocation in dynamic and static linking
* Dynamic linking
    * load time relocation
        * https://eli.thegreenplace.net/2011/08/25/load-time-relocation-of-shared-libraries
        * https://www.youtube.com/watch?v=wYmhUbhkbhQ
            -Demonstrates load time relocation
        * PIC - Position Independent code
            * https://www.youtube.com/watch?v=baNlnNt5OYg
            * https://eli.thegreenplace.net/2011/11/03/position-independent-code-pic-in-shared-librariesss
            * https://www.youtube.com/watch?v=fnbBKinIumw&list=PLyqSpQzTE6M-q0Xgn0icEHvUS7WQxvenv&index=14
                - Explains ASLR and load time relocation and PIC 
        * For Cortex-M
            * https://stackoverflow.com/questions/5619178/how-to-write-dynamic-loader-for-bare-metal-arm-application
                - Explains how to do jump tables (jumptables) using C struct
            * https://stackoverflow.com/questions/50655162/stm32-position-independent-binaries
                - Position independent code
            * https://github.com/embedded2014/elf-loader
            * https://github.com/martinribelotta/elfloader
                - ELF Loader
            * http://www.chibios.com/forum/viewtopic.php?f=3&t=1229
                - partial linking and comparison of different dynamic loading options
            * https://stackoverflow.com/questions/44632383/dynamically-load-code-on-embedded-target
                - Shows partial linking
                - In the ld documentation check out --relocatable and --just-symbols
            * https://static.docs.arm.com/ihi0044/f/IHI0044F_aaelf.pdf
                - ARM ELF Format







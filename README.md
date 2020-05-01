### STM32 Dynamic Linking

<br />
<p align="center">
  <p align="center">
    Minimal example of dynamic apps.
    <br />
    <a href="https://rgujju.github.io/STM32_Dynamic_Apps/html/index.html"><strong>Doxygen docs »</strong></a>
    <br />
  </p>
</p>

## About The Project
This is a minimal example of having dynamic apps with syscall functionality. It is divided into 2 parts, the kernel
and the app. The kernel is responsible for loading the app and handling syscall functionality. The syscall functionality
is implemented using jumptables. The app can then call APIs present in the kernel through this jump table.

#### Kernel
At the moment the kernel is nothing but the FreeRTOS and 2 functions, the ``SetLed`` and ``LoadApp``.
The syscall functionality is implemented in the **modules/sys_module** while the app loading is implemented in the 
**modules/app_loader**.  

The jumptable struct is ``sys_struct sys``. It should contain all the APIs (currently only the ``SetLed``) that 
the app can invoke in the kernel.

The kernel contains the actual code of ``SetLed``, This code does not get compiled into the app again. So this API
can be changed without needing to rebuild the app.

#### App
The app is compiled as a Position independent Code. 
The following flags are used:``-fpic -msingle-pic-base -mpic-register=r9 -mno-pic-data-is-text-relative -mlong-calls`` 
This creates an executable which can be run from any memory location given that the GOT address is in the register **R9**
gcc generates the executable in elf which contains a lot of information and is quite large for a mcu. So this elf file is 
converted into [TINF](https://github.com/rgujju/STM32_Dynamic_Apps/tree/master/elf2tinf), which can be easily loaded onto a mcu.  

The app invokes syscalls using the pointer ``sys_struct sys``. The actual address of the ``sys_struct sys`` is added when the app
is loaded. When the app invokes a syscall as follows ``sys->SetLed(led_num, led_status);`` the execution flow jumps into the actual
location of the ``SetLed`` function which was defined in the kernel.

The current example app in **apps/blinky** turns on the green LED while the kernel turns on the red LED on the STM32F429 DISC1 board.

To learn more about GOT and PIC refer the [Acknowledgements](#Acknowledgements), they do a much better job of explaining the concepts.

#### Why did I do this?
Mainly to learn about the GOT, PIC, memory layout of mcu, and a lot more. 
Gained a lot of knowledge from this project.

## Usage
This example is for the STM32F429 DISC1 board. But should be [portable](#Porting) to any other mcu.  
#### 1 Build userlib.a. This is a static library aka archive. The app will be linked to this archive.  
``mkdir -p build/userlib``  
``cd build/userlib``  
``cmake ../.. -DTARGET_GROUP=userlib -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cross.cmake``  
``make``  
A folder called **lib** will be created. This folder will contain everything required to build the app.  
It contains the header files and the userlib.a archive.   
Copy this folder to the apps folder.  
``cp -R lib ../../apps/``  

#### 2 Build app. A example app called blinky is provided.  
This app turn on LED1. There is some extra code in the example to purposely populate the data and bss sections  
And also to verify if the GOT is copied properly. ie global variable access.  
``cd apps``  
``mkdir build/``  
``cd build``  
``cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cross.cmake``  
``make``  
This should have created **blinky.elf** file. This file now needs to be converted into TINF.

#### 3 Create TINF of the app
``python3 ../../elf2tinf/elf2tinf.py --major 1 --minor 0 blinky.elf blinky``  
This should generate 2 files, the **blinky.tinf** and **blinky_tinf.h**   
**blink.tinf** is in a binary format which can be loaded over uart, ble, usb, etc.  
**blinky_tinf.h** is the same binary data in the form of a header file, so I can test it easily, 
without implementing the actual transfer of the binary to the mcu.  
More details about this tool is in the README.md in folder elf2tinf  

#### 4 Build the kernel
``mkdir -p build/debug``  
``cd build/debug``  
Build the kernel, this is the code that will actually load the app and run it  
``cmake ../.. -DTARGET_GROUP=kernel -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cross.cmake``  
``make``  
This will generate the **main.elf** file which needs to be loaded onto the board.

#### 5 Load main.elf to hardware
This can vary depending on your method.  
If you use gdb then simply, with something like openocd already running in the background then simply  
``make flash``  


## Documentation
User documentation is in header files  
To go deeper see the source files  
Online docs: https://rgujju.github.io/STM32_Dynamic_Apps/html/index.html


## Porting
- This project uses the STM32F429 mcu but should be portable to any mcu.
- Replace **components/STM32F4xx_HAL_Driver** and **include/stm32f4xx_hal_conf.h** with the HAL of your mcu.
- Replace **include/STM32F4xx** with the vendor files for your mcu. These files are basically the system, startup and header files of your mcu.
- The above 2 folders are provided by the vendor. In case of STM32, it is possible to use STM32CubeMX to generate them.
- Change *MCU Setup*, *HAL Setup* to match your mcu in the **CMakeLists.txt** file.
- Linker script **linker.ld** needs to be changed according to your mcu memory.


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



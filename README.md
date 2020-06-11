### STM32 Dynamic Linking
  <p align="center">
    Minimal example of dynamic apps.
    <br />
    <a href="https://rgujju.github.io/Dynamic_App_Loading/html/index.html"><strong>Doxygen docs »</strong></a>
    <br />
  </p>

## About The Project
This is a minimal example of having dynamic apps with syscall functionality. It is divided into 2 parts, the kernel
and the app. The kernel is responsible for loading the app and handling syscall functionality. The syscall functionality
is implemented using [zephyr syscalls](https://docs.zephyrproject.org/2.3.0/reference/usermode/syscalls.html).

#### Kernel
At the moment the kernel is nothing but zephyr and 2 functions, the ``SetLed`` and ``LoadApp``.
The syscall functionality is implemented in the **modules/sys_module** while the app loading is implemented in the 
**modules/app_loader**.  

The kernel contains the actual implementation of ``SetLed``, This code does not get compiled into the app again. So this API
can be changed without needing to rebuild the app.

There are some changes required in zephyr to support this. Have a look at [zephyr changes](#Changes required in zephyr) and a diff [here](https://github.com/rgujju/zephyr/commit/a7be4d5d662a2df6be4f895b54bd548aa269d74e)

#### App
The app is compiled as a Position independent Code. 
The following flags are used:``-fpic -msingle-pic-base -mpic-register=r9 -mno-pic-data-is-text-relative -mlong-calls``. 
This creates an executable which can be run from any memory location given that the GOT address is in the register **R9**.
This value is given as a parameter while calling the app, so this gets placed in **R0**. 
The ``AppStart`` function defined in **app_startup.s** is used to copy the value from **R0** to **R9**. This function contains the 
first instruction that gets executed when the app is started. Followed by the ``main`` function defined in app. This sequence is 
defined using the app linker script **app_base.ld**.  
gcc generates the executable in elf which contains a lot of information and is quite large for a mcu. So this elf file is 
converted into [TINF](https://github.com/rgujju/Dynamic_App_Loading/tree/master/elf2tinf), which can be easily loaded onto a mcu.  

The current example app blinky in [apps](https://github.com/rgujju/Dynamic_Apps) turns on the green LED while the kernel turns on the red LED on the STM32F429i-DISC1 board.
But should be able to port to any arm cortex m board.

To learn more about GOT and PIC refer the [Acknowledgements](#Acknowledgements), they do a much better job of explaining the concepts.

#### Why did I do this?
Mainly to learn about the GOT, PIC, memory layout of mcu, elf format, and a lot more. 
Gained a lot of knowledge from this project.

## Usage
This example is for the STM32F429i DISC1 board. But should be [portable](#Porting) to any other mcu.  
#### 1 Build libsys_module.a. This is a static library aka archive. The app will be linked to this archive.  
``mkdir build``  
``cd build``  
``cmake .. -DBOARD=stm32f429i_disc1 -DUSERLIB=1``  
``make userlib``  
A folder called **userlib** will be created. This folder will contain everything required to build the app.  
It contains the header and archive files.   
Copy this folder to the apps folder. The apps repo is at [Dynamic_Apps](https://github.com/rgujju/Dynamic_Apps)  
``cp -R userlib Dynamic_Apps/``  

#### 2 Build app. A example app called blinky is provided.  
This app turns on LED1. There is some extra code in the example to purposely populate the data and bss sections  
And also to verify if the GOT is copied properly. ie global variable access.  
``cd Dynamic_Apps/blinky``  
``mkdir build/``  
``cd build``  
``cmake ..``  
``make``  
This will create **blinky.elf** file. This file now needs to be converted into TINF.

#### 3 Create TINF of the app
``python3 <path>/elf2tinf/elf2tinf.py --major 1 --minor 0 <path>/blinky.elf blinky``  
This will generate 2 files, the **blinky.tinf** and **blinky_tinf.h**   
**blink.tinf** is in a binary format which can be loaded over uart, ble, usb, etc.  
**blinky_tinf.h** is the same binary data in the form of a header file, so the app can be tested easily, by
compiling into the kernel, without implementing the actual transfer of the binary to the mcu.  
More details about this tool is in the README.md in folder elf2tinf  

#### 4 Build the kernel
``cd build``  
Build the kernel, this is the code that will actually load the app and run it  
``cmake .. -DBOARD=stm32f429i_disc1 -DUSERLIB=0``  
``make clean`` Need to clean the build files of the userlib.
``make``
This will generate the **main.elf** file which needs to be loaded onto the board.

#### 5 Load main.elf to hardware
This can vary depending on your method. but if zephyr flash method works for you then simply  
``make flash``  


## Documentation
User documentation is in header files  
To go deeper see the source files  
Online docs: https://rgujju.github.io/Dynamic_App_Loading/html/index.html


## Porting
- This project uses the STM32F429 mcu but should be portable to any mcu.
- Zephyr makes porting extremely easy. Simply change the ``-DBOARD`` param given to cmake to match your board
- Modify the **CMakeLists.txt** in the app in [Dynamic_Apps](https://github.com/rgujju/Dynamic_Apps). mainly the **MCU** and **MCU_SPEC**
- To port to other architecture need to modify how the GOT BASE is passed to the app


## Limitations:
- Cannot reset thread stack to 0x00 while entering userpsace
- Statically allocated stack.
- No stack overflow detection yet


## Changes required in zephyr
Have a look at the diff [here](https://github.com/rgujju/zephyr/commit/a7be4d5d662a2df6be4f895b54bd548aa269d74e)

- Allocate space in the userspace thread stack for GOT, bss and data section of dynamic app. Right now I am just not
    resetting the thread stack to 0 or 0xaaaaaaaa while entering userspace so that it does not overwrite these sections. 

- The thread stack is statically allocated due to limiations in zephyr heap allocator. The current heap implementation
    cannot allocate stack which is aligned properly according to MPU

- Do not compile syscall APIs as static inline while building userlib, and set ``CONFIG_USERLIB=1`` and ``__ZEPHYR_USER__=1`` 
    - ``CONFIG_USERLIB=1`` is used to remove the ``static inline`` from the syscall declaration so that the API is exported.
    - ``__ZEPHYR_USER__=1`` is used to compile only the userspace version of the syscall 
    - The modified api definition is formed as follows

```
extern int8_t z_impl_some_api(uint8_t a, uint8_t b);
#ifndef CONFIG_USERLIB
static inline
#endif
int8_t some_api(uint8_t a, uint8_t b)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int8_t) arch_syscall_invoke2(*(uintptr_t *)&Led_Num, *(uintptr_t *)&Led_State, K_SYSCALL_SETLED);
	}
#endif
	compiler_barrier();
	return z_impl_SetLed(Led_Num, Led_State);
}

```

- The source of the C files not to be compiled in the userlib must be inside ``#ifndef CONFIG_USERLIB .. #endif``
- Only the generated syscall header files and its dependent header files 
  are required in the userlib so a typical module can be compiled as follows

module_name.c
```
#include <zephyr.h>
#include <device.h>
#include "<module_name>.h"

#ifndef CONFIG_USERLIB
 
int8_t z_impl_some_api(uint8_t a, uint8_t b){
    ...
}

int8_t z_vrfy_some_api(uint8_t a, uint8_t b){

}

#include <syscalls/some_api_mrsh.c>

#endif /*CONFIG_USERLIB*/

```

module_name.h
```
#ifndef CONFIG_USERLIB
_syscall int8_t some_api(uint8_t a, uint8_t b);

#endif

# The source for jumping to the syscall will 
get included from here
#include <syscalls/module_name.h>
```


5) Should be able to just add ``#include <syscalls/file_name.h>`` along with dependencies to include the APIs from that file.


## Acknowledgements
Following resources helped me:  
* https://github.com/bogdanm/udynlink
* https://github.com/pebble-dev/RebbleOS/blob/master/rcore/appmanager.c
* https://dl.acm.org/doi/abs/10.1145/1067170.1067188
* https://github.com/tock/tock
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



# LICENSE
#
# The MIT License (MIT)
#
# Copyright (c) 2020 Rohit Gujarathi https://github.com/rgujju
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


cmake_minimum_required(VERSION 3.11.1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
#-----------------------------
# Configuration
#-----------------------------
# TODO: Add support for building directly through zephyr by adding the -DBOARD flag
set(ZEPHYR_SDK_INSTALL_DIR $ENV{ZEPHYR_SDK_INSTALL_DIR})
set(CROSS_TOOLCHAIN ${ZEPHYR_SDK_INSTALL_DIR}/arm-zephyr-eabi/bin/arm-zephyr-eabi-)
#set(CROSS_TOOLCHAIN arm-none-eabi-)
# These values should be the same in kernel CMakeLists.txt file
set(MCU STM32F429xx)
set(MCU_SPEC  cortex-m4)
#~ set(FLOAT_SPEC "-mfloat-abi=hard -mfpu=fpv4-sp-d16") #TODO: Add support for fpu in the kernel as well.
set(APP_NAME blinky)
set(APP_COMPILE_FLAGS )
set(APP_SDK_ROOT /home/rohit/Documents/STM32/STM32-Dynamic-Linking/apps/)

#-----------------------------
# Compiler and generic options
#-----------------------------
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING 1)

set(CMAKE_C_COMPILER ${CROSS_TOOLCHAIN}gcc${TOOLCHAIN_EXT} CACHE INTERNAL "C Compiler")
set(CMAKE_CXX_COMPILER ${CROSS_TOOLCHAIN}g++${TOOLCHAIN_EXT} CACHE INTERNAL "C++ Compiler")
set(CMAKE_ASM_COMPILER ${CROSS_TOOLCHAIN}gcc${TOOLCHAIN_EXT} CACHE INTERNAL "ASM Compiler")
set(CMAKE_OBJCOPY ${CROSS_TOOLCHAIN}objcopy CACHE INTERNAL "Objcopy tool")
set(CMAKE_OBJDUMP ${CROSS_TOOLCHAIN}objdump CACHE INTERNAL "Objdump tool")
set(CMAKE_SIZE_UTIL ${CROSS_TOOLCHAIN}size CACHE INTERNAL "Size tool")
set(CMAKE_C_GDB ${CROSS_TOOLCHAIN}gdb-py CACHE INTERNAL "Debugger")
set(CMAKE_AR ${CROSS_TOOLCHAIN}gcc-ar CACHE INTERNAL "Assembler")
set(CMAKE_RANLIB ${CROSS_TOOLCHAIN}gcc-ranlib CACHE INTERNAL "Ranlib")

enable_language(C ASM)
# Project name
project(${APP_NAME})

#-------------------
# General Flags
#-------------------
set(OBJECT_GEN_FLAGS " \
-fno-builtin \
-Wall \
-ffunction-sections -fdata-sections \
-fomit-frame-pointer \
" CACHE INTERNAL "Common flags for C/CXX/ASM/Linker")

#-------------------
# CFLAGS
#-------------------
set(CMAKE_C_FLAGS " \
" CACHE INTERNAL "C Compiler options")

#-------------------
# ASMFLAGS for cross
#-------------------
set(CMAKE_ASM_FLAGS " \
-x assembler-with-cpp \
" CACHE INTERNAL "ASM Compiler options")

#-------------------
# LFLAGS for cross
#-------------------
set(CMAKE_EXE_LINKER_FLAGS " \
-Wl,-Map=${PROJECT_NAME}.map \
-Wl,--print-memory-usage \
-Wl,--gc-sections \
--specs=nosys.specs \
-nostdlib \
-Wl,--undefined=main \
-Wl,--emit-relocs \
" CACHE INTERNAL "Linker options")

#------------------
# Debug Flags
#------------------
#set(CMAKE_C_FLAGS_DEBUG "-Og -g -gdwarf-3 -gstrict-dwarf " CACHE INTERNAL "C Compiler options for debug build type")
#set(CMAKE_CXX_FLAGS_DEBUG "-Og -g -gdwarf-3 -gstrict-dwarf " CACHE INTERNAL "C++ Compiler options for debug build type")
#set(CMAKE_ASM_FLAGS_DEBUG "-Og -g -gdwarf-3 -gstrict-dwarf " CACHE INTERNAL "ASM Compiler options for debug build type")
#set(CMAKE_EXE_LINKER_FLAGS_DEBUG "" CACHE INTERNAL "Linker options for debug build type")

#------------------
# Release Flags
#-----------------
set(CMAKE_C_FLAGS_RELEASE "-O3 " CACHE INTERNAL "C Compiler options for release build type")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 " CACHE INTERNAL "C++ Compiler options for release build type")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "ASM Compiler options for release build type")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE " " CACHE INTERNAL "Linker options for release build type")

#------------------
# Position Independent Code Flags
#------------------
string(APPEND PIC_FLAGS "-fpic -msingle-pic-base -mpic-register=r9 -mno-pic-data-is-text-relative -mlong-calls")
#string(APPEND PIC_FLAGS "-fpic -mlong-calls")


message(STATUS "[INFO] Cross compiling for ${MCU}")
message(STATUS "[INFO] Building App ${APP_NAME}")

#-------------------
# General Flags for cross
#-------------------
string(APPEND OBJECT_GEN_FLAGS " \
-mcpu=${MCU_SPEC} \
-mthumb \
-mthumb-interwork \
-mabi=aapcs \
${FLOAT_SPEC} \
${PIC_FLAGS} \
")

#-------------------
# CFLAGS for cross
#-------------------
string(APPEND CMAKE_C_FLAGS " \
${OBJECT_GEN_FLAGS} \
")

#-------------------
# ASMFLAGS for cross
#-------------------
string(APPEND CMAKE_ASM_FLAGS " \
${OBJECT_GEN_FLAGS} \
")

#-------------------
# LFLAGS for cross
#-------------------
string(APPEND CMAKE_EXE_LINKER_FLAGS " \
")

######################################################################
# @Target: <app_name>
# @Brief: build the app
######################################################################

set(APP_LINKER_SCRIPT ${APP_SDK_ROOT}/app_base.ld)
string(APPEND CMAKE_EXE_LINKER_FLAGS "-T${APP_LINKER_SCRIPT} ")

# fetch app sources
file(GLOB APP_SOURCES ./*.c)

# Add executable target
add_executable(${APP_NAME}.elf
    ${APP_SDK_ROOT}/app_startup.s
    ${APP_SOURCES}
    )
# Generate disassembly
#~ objdump(${APP_NAME}.elf)

target_include_directories(${APP_NAME}.elf
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${APP_SDK_ROOT}/lib
    )

target_link_libraries(${APP_NAME}.elf
    PRIVATE
    ${APP_SDK_ROOT}/lib/libsys_module.a
    )

if(APP_COMPILE_FLAGS STREQUAL " ")
else()
    target_compile_definitions(${APP_NAME}.elf
        PUBLIC
        ${APP_COMPILE_FLAGS}
        )
endif()


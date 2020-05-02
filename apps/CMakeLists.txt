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

#################### CONFIGURABLE SECTION ###########################
enable_language(C ASM)
# Project name
project(apps)

## These values should be the same in kernel CMakeLists.txt file
set(MCU STM32F429xx)
set(MCU_SPEC  cortex-m4)
set(FLOAT_SPEC "-mfloat-abi=hard -mfpu=fpv4-sp-d16")

#####################################################################

set(APP_LINKER_SCRIPT app_base.ld)

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
"CACHE INTERNAL "Linker options")

#------------------
# Debug Flags
#------------------
set(CMAKE_C_FLAGS_DEBUG "-Og -g -gdwarf-3 -gstrict-dwarf " CACHE INTERNAL "C Compiler options for debug build type")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g -gdwarf-3 -gstrict-dwarf " CACHE INTERNAL "C++ Compiler options for debug build type")
set(CMAKE_ASM_FLAGS_DEBUG "-Og -g -gdwarf-3 -gstrict-dwarf " CACHE INTERNAL "ASM Compiler options for debug build type")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "" CACHE INTERNAL "Linker options for debug build type")

#------------------
# Release Flags
#-----------------
set(CMAKE_C_FLAGS_RELEASE "-Os " CACHE INTERNAL "C Compiler options for release build type")
set(CMAKE_CXX_FLAGS_RELEASE "-Os " CACHE INTERNAL "C++ Compiler options for release build type")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "ASM Compiler options for release build type")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE " " CACHE INTERNAL "Linker options for release build type")

#------------------
# Position Independent Code Flags
#------------------
string(APPEND PIC_FLAGS "-fpic -msingle-pic-base -mpic-register=r9 -mno-pic-data-is-text-relative -mlong-calls")
#string(APPEND PIC_FLAGS "-fpic -mlong-calls")

if(CMAKE_CROSSCOMPILING STREQUAL "1")
    # Control specific options if crosscompiling

    message(STATUS "[INFO] Cross compiling for ${MCU}")
    message(STATUS "[INFO] Building Apps")

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
    ${OBJECT_GEN_FLAGS} \
    ")

    ######################################################################
    # @Target: <app_name>
    # @Brief: build the app
    ######################################################################

    # Fetch the linker script for app
    set(LINKER_SCRIPT "${APP_LINKER_SCRIPT}")
    find_file(LINKER_SCRIPT_PATH
        NAMES "${LINKER_SCRIPT}"
        PATHS
        ${CMAKE_CURRENT_LIST_DIR}
        )

    if(DEFINED LINKER_SCRIPT_PATH)
        message(STATUS "[INFO] Using linker file at ${LINKER_SCRIPT_PATH}")
        string(APPEND CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT_PATH} ")
    else()
        message(FATAL_ERROR "[ERRR] Could not find linker script ${LINKER_SCRIPT}")
    endif()

    # Add new apps like this
    list(APPEND APPS_NAMES "blinky")
    list(APPEND APPS_COMPILE_FLAGS " ")

    # Declare all app targets
    list(LENGTH APP_NAMES APP_COUNT)
    math(EXPR P_APP_COUNT "${APP_COUNT} + 1")
    message(STATUS "[INFO] Adding ${P_APP_COUNT} apps")
    #math(EXPR APP_COUNT "${APP_COUNT} - 1")
    foreach(i RANGE ${APP_COUNT})
        list(GET APPS_NAMES ${i} APP_NAME)
        list(GET APPS_COMPILE_FLAGS ${i} APP_COMPILE_FLAGS)

        message(STATUS "[INFO] Adding app ${APP_NAME} with compile flags ${APP_COMPILE_FLAGS}")

        # fetch app sources
        file(GLOB APP_SOURCES ${APP_NAME}/*.c)

        # Add executable target
        add_executable(${APP_NAME}.elf
            ${PROJECT_SOURCE_DIR}/app_startup.s
            ${APP_SOURCES}
            )
        # Generate disassembly
        objdump(${APP_NAME}.elf)

        target_include_directories(${APP_NAME}.elf
            PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}/${APP_NAME}
            ${CMAKE_CURRENT_LIST_DIR}/lib
            )

        target_link_libraries(${APP_NAME}.elf
            PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/lib/libsys_module.a
            )

        if(APP_COMPILE_FLAGS STREQUAL " ")
        else()
        target_compile_definitions(${APP_NAME}.elf
                PUBLIC
                ${APP_COMPILE_FLAGS}
                )
        endif()
    endforeach()

    ######################################################################
    # @Target: gdb
    # @Brief: Start gdb client
    ######################################################################
    gdb()

else()
    message(FATAL_ERROR "CMAKE_CROSSCOMPILING is not set. Are you using cross compiler?")
endif()

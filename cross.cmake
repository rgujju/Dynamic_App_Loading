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

#-----------------------------
# Compiler and generic options
#-----------------------------
set(CROSS_TOOLCHAIN arm-none-eabi-)
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


#-----------------------------
# Helper functionss
#-----------------------------
# Add custom command to print firmware size in Berkley format
function(firmware_size target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} -B
        "${CMAKE_CURRENT_BINARY_DIR}/${target}"
        )
endfunction()

# Function to output disassembly 
function(objdump target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -Dz --source "${CMAKE_CURRENT_BINARY_DIR}/${target}" > ${target}.diss
        )
endfunction()

# Function to flash using gdb#
function(flash_gdb elf_file)
add_custom_target(flash_gdb DEPENDS ${elf_file})
add_custom_command(TARGET flash_gdb
    #COMMAND bash "-c" "pgrep -x \"openocd\" || (echo \"Please start openocd\" && exit -1)"
    COMMAND echo "Starting GDB client and loading ${elf_file} and outputting gdbDashboard to $(GDB_TTY)"
    COMMAND ${CMAKE_C_GDB} ${elf_file}
    -ex "target extended :3333"
    -ex "dashboard -output $(GDB_TTY)"
    -ex "load ${elf_file}"
    -ex "monitor arm semihosting enable"
    )
endfunction()

# Function to flash using gdb
function(gdb)
add_custom_target(gdb)
add_custom_command(TARGET gdb
    #COMMAND bash "-c" "pgrep -x \"openocd\" || (echo \"Please start openocd\" && exit -1)"
    COMMAND echo "Starting GDB client and outputting gdbDashboard to $(GDB_TTY)"
    COMMAND ${CMAKE_C_GDB} ${elf_file}
    -ex "target extended :3333"
    -ex "dashboard -output $(GDB_TTY)"
    -ex "monitor arm semihosting enable"
    )
endfunction()



#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)



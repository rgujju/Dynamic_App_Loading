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

# The project name
set(PROJECT_NAME main CACHE STRING "Name of the project")
# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number")
set(PROJECT_BASE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr HINTS $ENV{ZEPHYR_BASE})
#include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)

project(${PROJECT_NAME})
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

######################################################################
# @Target: Zephyr application
# @Brief: Build the kernel for the project
#         rwatch kernel is built as zephyr application
######################################################################
file(GLOB MAIN_SRC
    "src/*.c"
    )
target_include_directories(app
    PUBLIC
	src/
    )
target_compile_definitions(app
    PUBLIC
    ${COMPILE_DEFS}
    )
target_sources(app
    PRIVATE
    ${MAIN_SRC}
    )


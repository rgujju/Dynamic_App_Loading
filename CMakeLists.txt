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

set(PROJECT_NAME main CACHE STRING "Name of the project")
# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number")

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
-Wl,--gc-sections \
" CACHE INTERNAL "Linker options")

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
set(CMAKE_C_FLAGS_RELEASE "-Os -flto " CACHE INTERNAL "C Compiler options for release build type")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto " CACHE INTERNAL "C++ Compiler options for release build type")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "ASM Compiler options for release build type")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto " CACHE INTERNAL "Linker options for release build type")

#-------------------
# General Flags for native
#-------------------
#    string(APPEND OBJECT_GEN_FLAGS " \
#    --coverage \
#    ")

# Build the kernel
if(TARGET_GROUP STREQUAL kernel)
	message(STATUS "[INFO] Building kernel target group")

	# Control building kernel or userlib
	if(USERLIB)
		#set(RKERNEL 0)
	else()
		set(RKERNEL RKERNEL=1)
	endif()

	message(STATUS "[INFO] RKERNEL is: ${RKERNEL}")

	#------------------
	# Modules Setup
	#------------------
	list(APPEND ZEPHYR_EXTRA_MODULES
		${CMAKE_CURRENT_SOURCE_DIR}/modules/sys_module
		${CMAKE_CURRENT_SOURCE_DIR}/modules/app_loader
		)

	#-------------------
	# Common macros for kernel and its modules
	#-------------------
	set(COMPILE_DEFS
		${RKERNEL}
		)

	set(MODULE_COMPILE_DEFS
		${COMPILE_DEFS}
		)

	#list(APPEND SYSCALL_INCLUDE_DIRS
	#	${CMAKE_CURRENT_SOURCE_DIR}/modules/sys_module/zephyr
	#	)

	#find_package(Zephyr HINTS $ENV{ZEPHYR_BASE})
	include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)

	project(${PROJECT_NAME})
	mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

	# Define the linker script location
	#set(KERNEL_LINKER_SCRIPT linker.ld)

	#set(LINKER_SCRIPT "${KERNEL_LINKER_SCRIPT}")
	#find_file(LINKER_SCRIPT_PATH
	#    NAMES "${LINKER_SCRIPT}"
	#    PATHS
	#    ${CMAKE_CURRENT_LIST_DIR}
	#    )

	#if(DEFINED LINKER_SCRIPT_PATH)
	#    message(STATUS "[INFO] Using linker file at ${LINKER_SCRIPT_PATH}")
	#    string(APPEND CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT_PATH} ")
	#else()
	#    message(FATAL_ERROR "[ERRR] Could not find linker script ${LINKER_SCRIPT}")
	#endif()

	######################################################################
	# @Target: Zephyr application
	# @Brief: Build the kernel for the project
	######################################################################

	#-------------------
	# Main elf
	#-------------------
	file(GLOB MAIN_SRC
		"src/*.c"
		)
	target_include_directories(app
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
		)
	#target_link_libraries(app
	#PUBLIC
	#sys_module
	#)
	target_compile_definitions(app
		PUBLIC
		${COMPILE_DEFS}
		)
	target_sources(app
		PRIVATE
		${MAIN_SRC}
		)

	######################################################################
	# @Target: doc
	# @Brief: Generate doxygen documentation
	######################################################################
	# check if Doxygen is installed
	find_package(Doxygen)
	if (DOXYGEN_FOUND)
		# set input and output files
		set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
		set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

		# exclude folders while making documentation
		# should be separated by spaces
		set(DOXYGEN_EXCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/components/* ${CMAKE_CURRENT_SOURCE_DIR}/build/* ${CMAKE_CURRENT_SOURCE_DIR}/${MCU_DIR}* ${CMAKE_CURRENT_SOURCE_DIR}/apps/lib")

		# set mainpage in doxygen
		set(DOXYGEN_MAIN_PAGE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

		# request to configure the file
		configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
		message("Doxygen build started")

		# Generate doxygen documentation
		# And copy to docs folder on project root so github pages can display docs
		add_custom_target(docs
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
			COMMAND ${CMAKE_COMMAND} -E copy_directory html ${PROJECT_SOURCE_DIR}/docs/html
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM
			)
	else (DOXYGEN_FOUND)
		message("Doxygen need to be installed to generate the doxygen documentation")
	endif (DOXYGEN_FOUND)

	# Add additional files to the make clean
	set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
		"${PROJECT_NAME}.map"
		"${CMAKE_CURRENT_BINARY_DIR}/html"
		"${CMAKE_CURRENT_BINARY_DIR}/latex"
		)

	#elseif(TARGET_GROUP STREQUAL userlib)

	#message(STATUS "[INFO] Building userlib target group")
	#------------------
	# Modules Setup
	#------------------
	# Build sys_module without -DRKERNEL, this will build the
	# jumptable pointer so app can call APIs in the kernel
	#list(APPEND ZEPHYR_EXTRA_MODULES
	#    ${CMAKE_CURRENT_SOURCE_DIR}/modules/sys_module
	#    )

	#set(MODULE_COMPILE_DEFS
	#    )

	#list(APPEND SYSCALL_INCLUDE_DIRS
	#	${CMAKE_CURRENT_SOURCE_DIR}/modules/sys_module/zephyr
	#	)

	#find_package(Zephyr HINTS $ENV{ZEPHYR_BASE})
	#include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)

	# copy the sys_module lib and header files to lib folder for easy distribution
	add_custom_target(userlib DEPENDS sys_module)
	add_custom_command(TARGET userlib
		COMMAND ${CMAKE_COMMAND} -E make_directory userlib
		COMMAND ${CMAKE_COMMAND} -E copy modules/sys_module/libsys_module.a userlib
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/modules/sys_module/*.h userlib
		)

elseif(TARGET_GROUP STREQUAL test)

	#-------------------
	# Common macros for testing modules
	#-------------------
	#set(MODULE_COMPILE_DEFS
	#    ${MCU}
	#    HSE_VALUE=${HSE_VAL}
	#    KERNEL=${KERNEL}
	#    )
	#set(MODULE_LINK_LIBS
	#    rtos
	#    hal
	#    )
	add_subdirectory(modules)

	add_library(unity STATIC
		components/unity/unity.c
		)

	target_include_directories(unity PUBLIC
		components/unity/
		)

	enable_testing()
	include(CTest)
	add_subdirectory(test)

	set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
	include(CodeCoverage)
	setup_target_for_coverage_lcov(
		NAME coverage
		EXCLUDE "/usr/include/x86_64-linux-gnu/*" "components/unity/*"
		EXECUTABLE ctest -VV
		)
else()
	message(FATAL_ERROR "Given TARGET_GROUP unknown")
endif()

# Unset all cache
unset(CMAKE_BUILD_TYPE)


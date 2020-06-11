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

set(MODULE_NAME sys_module)

file(GLOB SOURCES ./*.c)

zephyr_include_directories(
		${CMAKE_CURRENT_LIST_DIR}/..
		${CMAKE_SOURCE_DIR}/include
		)
zephyr_library_named(${MODULE_NAME})
zephyr_library_sources(
	${SOURCES}
	)
if(MODULE_COMPILE_DEFS)
	zephyr_compile_definitions(
		${MODULE_COMPILE_DEFS}
		)
endif()

#if(MODULE_LINK_LIBS)
#    target_link_libraries(${MODULE_NAME}
#        PRIVATE
#        ${MODULE_LINK_LIBS}
#        )
#endif()

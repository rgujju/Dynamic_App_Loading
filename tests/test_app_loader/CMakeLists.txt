
cmake_minimum_required(VERSION 3.11.1)
# Only set the name of the module to test and project base with respect
# to the current test folder
set(MODULE_NAME app_loader)
set(PROJECT_BASE ${CMAKE_CURRENT_SOURCE_DIR}/../../)

#################### DO NOT CHANGE ANYTHING BELOW THIS ########################

set(PROJECT_NAME test_${MODULE_NAME})
project(${PROJECT_NAME})

find_path (MODULE_PATH ${MODULE_NAME}.c
    HINTS ${PROJECT_BASE}/modules/${MODULE_NAME})

if(${MODULE_PATH} STREQUAL MODULE_PATH-NOTFOUND)
    message(FATAL_ERROR "Unable to find module ${MODULE_NAME}")
endif()
message("[INFO] Found module path: ${MODULE_PATH}")

find_path (INCLUDE_PATH utilities.h HINTS ${PROJECT_BASE}/include)

if(${INCLUDE_PATH} STREQUAL INCLUDE_PATH-NOTFOUND)
    message(FATAL_ERROR "Unable to find utilities.h path")
endif()

message("[INFO] Found utilities.h path: ${INCLUDE_PATH}")

#find_package(ZephyrUnittest HINTS $ENV{ZEPHYR_BASE})
find_package(Zephyr HINTS $ENV{ZEPHYR_BASE})

add_subdirectory(${MODULE_PATH} ${MODULE_NAME})
file(GLOB SOURCES *.c)
target_include_directories(app PUBLIC ${INCLUDE_PATH})
target_sources(app PRIVATE ${SOURCES})
target_link_libraries(app PRIVATE ${MODULE_NAME})

#FILE(GLOB app_sources *.c)
#target_sources(app PRIVATE ${app_sources})
#target_include_directories(app
#PUBLIC
#${CMAKE_CURRENT_SOURCE_DIR}/../include
#${CMAKE_CURRENT_SOURCE_DIR}/../modules
#)

###############################################################################
# Top level CMakeList for building the TMU source code
###############################################################################
cmake_minimum_required(VERSION 3.15)

# Convert CMake flags to compiler flags
if(EVT_CORE_LOG_ENABLE)
    add_compile_definitions(EVT_CORE_LOG_ENABLE)
endif()

# Handle default selection of the target device
if(NOT TARGET_DEV)
    set(TARGET_DEV "STM32F334x8")
endif()

# Enable use of HAL Drivers
add_compile_definitions(USE_HAL_DRIVER)

set(EVT_CORE_DIR      ${CMAKE_SOURCE_DIR}/libs/EVT-core)

# Link to the EVT-core library
add_subdirectory(libs/EVT-core/)

include(CMakeDependentOption)
include(${EVT_CORE_DIR}/cmake/evt-core_compiler.cmake)
include(${EVT_CORE_DIR}/cmake/evt-core_install.cmake)

###############################################################################
# Project Setup
###############################################################################
set(BOARD_LIB_NAME TMU)
if("${BOARD_LIB_NAME}" STREQUAL BOARD_NAME)
    message(FATAL_ERROR
            "You must set the template project name in the top-level CMakeLists.txt")
endif()

file(STRINGS version.txt BOARD_VERSION)

project(${BOARD_LIB_NAME}
        VERSION ${BOARD_VERSION}
        LANGUAGES CXX C
        )

add_library(${PROJECT_NAME} STATIC)

# Add sources
target_sources(${PROJECT_NAME} PRIVATE
        src/TMU.cpp
        src/dev/MAX31855.cpp
        )

###############################################################################
# Handle dependencies
###############################################################################
target_link_libraries(${PROJECT_NAME}
        PUBLIC EVT
        )

###############################################################################
# Install and expose library
###############################################################################

install_and_expose(${PROJECT_NAME})

###############################################################################
# Build Target Code
###############################################################################
add_subdirectory(targets)

###################################################################################
# Apache Software License 2.0
#
# Copyright 2023-2025, Trevor James Smith, David Huard
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###################################################################################
cmake_minimum_required(VERSION 3.26)

# Set default compile options
option(COMPILE_LIB "If ON, will create a dynamic lib file (default: OFF)" OFF)
option(COMPILE_EXE "If ON, will create a executable file (default: ON)" ON)
option(PYTHON, "If ON, will create a share library for python (default: OFF)" ON)

# Compiler flags for Windows
if(WIN32)
    message(STATUS "Compiling for Windows")
    message(STATUS "Setting MSVC flags for Windows")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
elseif(UNIX)
    message(STATUS "Compiling for Unix-like OS")
endif()

# Add CMake module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set C++ standard explicitly
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type in CMake if skbuild is set
message(STATUS "CMAKE_BUILD_TYPE set to '${CMAKE_BUILD_TYPE}'")

# Setup Raven Project
project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX VERSION ${SKBUILD_PROJECT_VERSION})

# Set the RavenHydroFramework source code directory
set(raven_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/RavenHydroFramework")

if(NOT EXISTS ${raven_SOURCE_DIR} OR ${ALWAYS_DOWNLOAD})
    if(NOT EXISTS ${raven_SOURCE_DIR})
        message(STATUS "Sources ${raven_SOURCE_DIR} not found: fetching Raven source files from remote")
    endif()
    if(${ALWAYS_DOWNLOAD})
        message(STATUS "ALWAYS_DOWNLOAD set as True: Forcing download verification")
    endif()

    include(FetchContent)
    # Define source library location
    FetchContent_Declare(
        raven
        GIT_REPOSITORY ${RAVEN_GIT_REPO}
        GIT_TAG ${RAVEN_GIT_TAG}
        )
    # Fetch remote source files without building them
    FetchContent_Populate(raven)
else()
    message(STATUS "Sources found: ${raven_SOURCE_DIR}")
endif()

# Find header & source
include_directories(${raven_SOURCE_DIR})
file(GLOB HEADER "${raven_SOURCE_DIR}/src/*.h")
file(GLOB SOURCE "${raven_SOURCE_DIR}/src/*.cpp")

# Add Python files
if(PYTHON)
    set(PYBIND11_NEWPYTHON ON)
    find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
    find_package(pybind11 CONFIG REQUIRED)
    pybind11_add_module(libraven MODULE "${raven_SOURCE_DIR}/src/py/libraven.cpp")
    target_include_directories(libraven PUBLIC "${raven_SOURCE_DIR}/src")
    target_compile_features(libraven PUBLIC cxx_std_11)
    target_compile_definitions(libraven PUBLIC BMI_LIBRARY)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(libraven PRIVATE "-Wno-deprecated")
        message(STATUS "Modified Python module compile flags with '-Wno-deprecated'")
    endif()
endif()

# File extension is OS dependent (Linux: none, Windows: .exe)
if(WIN32)
    set(EXE_EXTENSION ".exe")
    set(LIB_EXTENSION ".dll")
else()
    set(EXE_EXTENSION "")
    set(LIB_EXTENSION ".so")
endif()

# Create executable
if(COMPILE_EXE)
    add_executable(raven
        ${HEADER}
        ${SOURCE}
    )
    set_target_properties(raven PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(raven PUBLIC "${raven_SOURCE_DIR}/src")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(raven PRIVATE "-Wno-deprecated")
        message(STATUS "Modified binary compile flags with '-Wno-deprecated'")
    endif()

    # FIXME: BMI_LIBRARY is not yet defined in the RavenHydroFramework
    # target_compile_definitions(raven PUBLIC BMI_LIBRARY)
endif()

# Add source files
source_group("Header Files" fileS ${HEADER})
source_group("Source Files" fileS ${SOURCE})

if(USE_NETCDF)
    message(STATUS "Using NetCDF library")
    # Find HDF5 library
    find_package(HDF5 REQUIRED)
    # Find NetCDF library using custom helper script
    list(APPEND CMAKE_MODULE_PATH ${HELPERS})
    find_package(NetCDF REQUIRED)
    # Find ZLib library
    find_package(ZLIB REQUIRED)

    # Link relevant NetCDF libraries to build
    include_directories(${NetCDF_INCLUDE_DIRS})
    if(PYTHON)
        target_compile_options(libraven PUBLIC "-Dnetcdf")
        target_link_libraries(libraven PUBLIC ${NetCDF_LIBRARIES})
    endif()
    if(COMPILE_EXE)
        target_compile_options(raven PUBLIC "-Dnetcdf")
        target_link_libraries(raven PUBLIC ${NetCDF_LIBRARIES})
    endif()
endif()

# Install library to environment Python /lib
install(TARGETS libraven LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})

# Install binary to environment /bin
if(COMPILE_EXE)
    if(WIN32)
        install(TARGETS raven DESTINATION ${SKBUILD_SCRIPTS_DIR})
    else()
        add_custom_command(
            TARGET raven POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/raven" "${SKBUILD_SCRIPTS_DIR}/raven")
    endif()

    if(${raven_POPULATED})
        # Add license for RavenHydroFramework to source distribution after building library from sources (fetched via Cmake_FetchContent)
        add_custom_command(
            TARGET raven POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${raven_SOURCE_DIR}/LICENSE" "${SKBUILD_PLATLIB_DIR}/RavenHydroFramework/LICENSE")
    endif()
endif()

# Unset CMake variables to avoid polluting the cache
unset(COMPILE_LIB CACHE)
unset(COMPILE_EXE CACHE)

#    Copyright 2023 Ouranos Inc. and contributors
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
cmake_minimum_required(VERSION 3.26)

# Set default compile options
option(COMPILE_LIB "If ON, will create a dynamic lib file (default: OFF)" OFF)
option(COMPILE_EXE "If ON, will create a executable file (default: ON)" ON)
option(PYTHON, "If ON, will create a share library for python (default: OFF)" ON)

# Add CMake module path
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set C++ standard explicitly
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setup Raven Project
PROJECT(${SKBUILD_PROJECT_NAME} LANGUAGES CXX VERSION ${SKBUILD_PROJECT_VERSION})

# Set the RavenHydroFramework source code directory
SET(raven_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/RavenHydroFramework")

# Remove deprecation warnings for GCC
IF(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wno-deprecated)
    message(STATUS "Modified compile flags with '-Wno-deprecated'")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(NOT EXISTS ${raven_SOURCE_DIR} OR ${ALWAYS_DOWNLOAD})
    IF(NOT EXISTS ${raven_SOURCE_DIR})
        message(STATUS "Sources ${raven_SOURCE_DIR} not found: fetching Raven source files from remote")
    ENDIF()
    IF(${ALWAYS_DOWNLOAD})
        message(STATUS "ALWAYS_DOWNLOAD set as True: Forcing download verification")
    ENDIF()

    include(FetchContent)
    # Define source library location
    FetchContent_Declare(
        raven
        GIT_REPOSITORY ${RAVEN_GIT_REPO}
        GIT_TAG ${RAVEN_GIT_TAG}
        )
    # Fetch remote source files without building them
    FetchContent_Populate(raven)
ELSE()
    message(STATUS "Sources found: ${raven_SOURCE_DIR}")
ENDIF()

# Add NetCDF support
IF(USE_NETCDF)
    # Add NetCDF support flag
    add_compile_options(-Dnetcdf)
    message(STATUS "Modified compile flags with '-Dnetcdf'")

    # Find NetCDF library
    list(APPEND CMAKE_MODULE_PATH ${HELPERS})
    find_package(NetCDF REQUIRED)

    # Find ZLib library
    find_package(ZLIB)
ENDIF()

# Find header & source
include_directories(${raven_SOURCE_DIR})
FILE(GLOB HEADER "${raven_SOURCE_DIR}/src/*.h")
FILE(GLOB SOURCE "${raven_SOURCE_DIR}/src/*.cpp")

# Add Python files
if (PYTHON)
  SET(PYBIND11_NEWPYTHON ON)
  find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
  find_package(pybind11 CONFIG REQUIRED)
  pybind11_add_module(libraven MODULE "${raven_SOURCE_DIR}/src/py/libraven.cpp")
  target_include_directories(libraven PUBLIC "${raven_SOURCE_DIR}/src")
  target_compile_features(libraven PUBLIC cxx_std_11)
  target_compile_definitions(libraven PUBLIC BMI_LIBRARY)
endif()

# Create executable; File extension is OS dependent (Linux: none, Windows: .exe)
IF(COMPILE_EXE)
    add_executable(raven
        ${HEADER}
        ${SOURCE}
    )
    target_include_directories(raven PUBLIC "${raven_SOURCE_DIR}/src")

    # FIXME: The issue is that one of these flags determines which files are included in the build, See RavenMain.cpp
    # Add flag to indicate which version of ExitGracefully to use (set in RavenMain.cpp)
    target_compile_definitions(raven PUBLIC STANDALONE)
    # target_compile_definitions(raven PUBLIC BMI_LIBRARY)
    set_target_properties(raven PROPERTIES LINKER_LANGUAGE CXX)
ENDIF()

# Add source files
source_group("Header Files" FILES ${HEADER})
source_group("Source Files" FILES ${SOURCE})

IF(USE_NETCDF)
    # Link relevant NetCDF libraries to build
    include_directories(${NetCDF_INCLUDE_DIRS})
    IF(PYTHON)
        target_link_libraries(libraven PUBLIC ${NetCDF_LIBRARIES})
    ENDIF()
    IF(COMPILE_EXE)
        target_link_libraries(raven PUBLIC ${NetCDF_LIBRARIES})
    ENDIF()
ENDIF()

# Install library to environment Python /lib
INSTALL(TARGETS libraven LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})

# Install binary to environment /bin
IF(COMPILE_EXE)
    IF(WIN32)
        INSTALL(TARGETS raven DESTINATION ${SKBUILD_SCRIPTS_DIR})
    ELSE()
        add_custom_command(
            TARGET raven POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/raven" "${SKBUILD_SCRIPTS_DIR}/raven")
    ENDIF()

    IF(NOT ${SKBUILD_STATE} STREQUAL "wheel")
        # Add license for RavenHydroFramework to source distribution after building library from sources (not wheel)
        add_custom_command(
            TARGET raven POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/RavenHydroFramework"
            COMMAND ${CMAKE_COMMAND} -E copy "${raven_SOURCE_DIR}/LICENSE" "${SKBUILD_PLATLIB_DIR}/RavenHydroFramework/LICENSE")
    ENDIF()
ENDIF()

# Unset CMake variables to avoid polluting the cache
unset(COMPILE_LIB CACHE)
unset(COMPILE_EXE CACHE)

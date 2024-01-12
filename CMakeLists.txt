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
cmake_minimum_required(VERSION 3.24...3.27)

# Setup Raven Project
PROJECT(${SKBUILD_PROJECT_NAME} LANGUAGES CXX VERSION ${SKBUILD_PROJECT_VERSION})

# Set the RavenHydroFramework source directory
SET(raven_SOURCE_DIR RavenHydroFramework)

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

# Find header & source
FILE(GLOB HEADER "${raven_SOURCE_DIR}/src/*.h")
FILE(GLOB SOURCE "${raven_SOURCE_DIR}/src/*.cpp")

# Add Python files
include_directories(${raven_SOURCE_DIR}/src)
SET(PYBIND11_NEWPYTHON ON)
find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_core MODULE src/main.cpp)

# Add flag to indicate which version of ExitGracefully to use (set in RavenMain.cpp)
# target_compile_definitions(_core PUBLIC STANDALONE)

target_compile_definitions(_core PRIVATE RAVEN_VERSION_INFO=${RAVEN_VERSION} REQUIRED)
target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})
target_compile_features(_core PUBLIC cxx_std_11)

# Add NetCDF support
IF(USE_NETCDF)
    # Add NetCDF support flag
    add_compile_options(-Dnetcdf)
    message(STATUS "Modified compile flags with '-Dnetcdf'")

    # Find NetCDF library
    list(APPEND CMAKE_MODULE_PATH ${HELPERS})
    find_package(NetCDF REQUIRED)

    # Find ZLib library
    find_package(ZLIB REQUIRED)
ENDIF()

# Create executable; File extension is OS dependent (Linux: none, Windows: .exe)
IF(COMPILE_EXE)
    add_executable(raven
        ${SOURCE}
        ${HEADER}
    )
    set_target_properties(raven PROPERTIES LINKER_LANGUAGE CXX)
ENDIF()

# Add source files to executable
source_group("Header Files" FILES ${HEADER})
source_group("Source Files" FILES ${SOURCE})

IF(USE_NETCDF)
    # Link relevant NetCDF libraries to build
    include_directories(${NetCDF_INCLUDE_DIRS})
    target_link_libraries(_core PUBLIC ${NetCDF_LIBRARIES})
    IF(COMPILE_EXE)
        target_link_libraries(raven PUBLIC ${NetCDF_LIBRARIES})
    ENDIF()
ENDIF()

# Install library to environment Python /lib
INSTALL(TARGETS _core LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})

# Install binary to environment /bin
IF(COMPILE_EXE)
    IF(WIN32)
        INSTALL(TARGETS raven DESTINATION ${SKBUILD_SCRIPTS_DIR})
    ELSE()
        add_custom_command(
            TARGET raven POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/raven ${SKBUILD_SCRIPTS_DIR}/raven)
    ENDIF()
ENDIF()

# Unset CMake variables to avoid polluting the cache
unset(COMPILE_LIB CACHE)
unset(COMPILE_EXE CACHE)

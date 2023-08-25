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

SET(raven_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/raven-src)

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
        URL ${RAVEN_URL}
        URL_HASH SHA256=${RAVEN_SHA256}
        )
    # Fetch remote source files without building them
    FetchContent_Populate(raven)
ELSE()
    message(STATUS "Sources found: ${raven_SOURCE_DIR}")
ENDIF()

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

# Add Python files
SET(PYBIND11_NEWPYTHON ON)
find_package(Python COMPONENTS REQUIRED Interpreter Development)
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_core MODULE src/main.cpp)
target_compile_definitions(_core PRIVATE RAVEN_VERSION_INFO=${RAVEN_VERSION} REQUIRED)
target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

# Find header & source
FILE(GLOB HEADER "${raven_SOURCE_DIR}/*.h")
FILE(GLOB SOURCE "${raven_SOURCE_DIR}/*.cpp")

# Add source files to build
add_executable(raven
  ${SOURCE}
  ${HEADER}
)
source_group("Header Files" FILES ${HEADER})
source_group("Source Files" FILES ${SOURCE})

# Link Anaconda environment libraries if available, otherwise use system libraries
# if(NOT "${CONDA_LIBRARY_PATH}" STREQUAL "")
#     include_directories(${CONDA_LIBRARY_PATH})
# else()
#     include_directories(${NetCDF_INCLUDE_DIRS})
# endif()

IF(USE_NETCDF)
    # Link relevant NetCDF libraries to build
    include_directories(${NetCDF_INCLUDE_DIRS})
    target_link_libraries(raven PUBLIC ${NetCDF_LIBRARIES})
ENDIF()

target_compile_features(raven PUBLIC cxx_std_11)
set_target_properties(raven PROPERTIES LINKER_LANGUAGE CXX)

# Install library to environment Python /lib
INSTALL(TARGETS _core LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})

# Install binary to environment /bin
IF(WIN32)
  install(TARGETS raven DESTINATION ${SKBUILD_SCRIPTS_DIR})
ELSE()
  add_custom_command(
          TARGET raven POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy
                  ${CMAKE_CURRENT_BINARY_DIR}/raven
                  ${SKBUILD_SCRIPTS_DIR}/raven)
ENDIF()

# Add License for Raven after building library
add_custom_command(
        TARGET raven POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${raven_SOURCE_DIR}/LICENSE
                ${SKBUILD_SCRIPTS_DIR}/raven_COPYING)

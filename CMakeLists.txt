cmake_minimum_required(VERSION 3.24...3.26)

# Print message to stdout
include(CMakePrintHelpers)

# Setup Raven Project
PROJECT(${SKBUILD_PROJECT_NAME} LANGUAGES CXX VERSION ${SKBUILD_PROJECT_VERSION})

# Fetch remote source files
include(FetchContent)
FetchContent_Populate(
    RavenSource
    URL ${RAVEN_URL}
    URL_HASH SHA256=${RAVEN_SHA256}
    SOURCE_DIR RavenSource
)

# Find NetCDF library
list(APPEND CMAKE_MODULE_PATH ${HELPERS})
find_package(NetCDF REQUIRED)
cmake_print_variables(NetCDF_INCLUDE_DIRS)

# Find ZLib library
find_package(ZLIB REQUIRED)

# Add Python files
set(PYBIND11_NEWPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_core MODULE src/main.cpp)
target_compile_definitions(_core PRIVATE RAVEN_VERSION_INFO=${RAVEN_VERSION} REQUIRED)
target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

# Find header & source
file(GLOB HEADER "${ravensource_SOURCE_DIR}/*.h")
file(GLOB SOURCE "${ravensource_SOURCE_DIR}/*.cpp")

# Add source files to build
add_executable(raven
  ${SOURCE}
  ${HEADER}
)
source_group("Header Files" FILES ${HEADER})
source_group("Source Files" FILES ${SOURCE})

# Link relevant libraries to build
target_link_libraries(raven PRIVATE NetCDF::NetCDF)
target_compile_features(raven PUBLIC cxx_std_11)
set_target_properties(raven PROPERTIES LINKER_LANGUAGE CXX)

# Install binary to environment $PATH
install(TARGETS raven DESTINATION ${SKBUILD_SCRIPTS_DIR})
install(TARGETS _core LIBRARY DESTINATION ${SKBUILD_PROJECT_NAME})

# Add License for Raven after building library
add_custom_command(
        TARGET raven POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${ravensource_SOURCE_DIR}/LICENSE
                ${SKBUILD_SCRIPTS_DIR}/raven_COPYING)

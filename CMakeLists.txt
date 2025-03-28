################################################################################
#                                                                              #
# Copyright (C) 2019 Fondazione Istitito Italiano di Tecnologia (IIT)          #
# All Rights Reserved.                                                         #
#                                                                              #
################################################################################

# @author Luca Tricerri <luca.tricerri@iit.it>

cmake_minimum_required(VERSION 3.5)

project(blockTest
        LANGUAGES C CXX
        VERSION 2.3.8)

set(CMAKE_AUTOMOC ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if (CMAKE_VERSION VERSION_LESS 3.11)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake-3.11)
endif()

include(GNUInstallDirs)

option(COMPILE_SCRIPTBUILDER "Compile the UI for creating the test" ON)
option(BLOCKTEST_USE_EXTERNAL_PUGIXML "Set to on if you want to use an external pugixml" OFF)
option(USE_SERIAL_LIB "Compile with serial library" OFF)
option(ENABLE_MSVC_WARNINGS "Enable the compilation warnings in windows" ON)

if(BLOCKTEST_USE_EXTERNAL_PUGIXML)
  find_package(pugixml REQUIRED)
else()
  add_subdirectory(extern/pugixml)
endif()

set(BOOST_MIN_VERSION "1.65.0")

find_package(YCM REQUIRED)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS filesystem system)
find_package(Qt5 QUIET COMPONENTS Core Gui Widgets)

if(USE_SERIAL_LIB)
  find_package(serial)
endif()

# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
# See https://cmake.org/cmake/help/v3.4/variable/CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS.html
# See https://blog.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Under MSVC, we set CMAKE_DEBUG_POSTFIX to "d" to add a trailing "d" to library
# built in debug mode. In this Windows user can compile, build and install the
# library in both Release and Debug configuration avoiding naming clashes in the
# installation directories.
if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

if (NOT Boost_FOUND)
      message(FATAL_ERROR "Fatal error: Boost (version >= ${BOOST_MIN_VERSION}) required.\n")
endif (NOT Boost_FOUND)

# Control where libraries and executables are placed during the build.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if (CMAKE_VERSION VERSION_LESS 3.5)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake-3.5)
endif()

# Encourage user to specify a build type (e.g. Release, Debug, etc.), otherwise set it to Debug.
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Debug' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Debug")
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Build position independent code.
# Position Independent Code (PIC) is commonly used for shared libraries so that
# the same shared library code can be loaded in each program address space in a
# location where it will not overlap with any other uses of such memory.
# In particular, this option avoids problems occurring when a process wants to
# load more than one shared library at the same virtual address.
# Since shared libraries cannot predict where other shared libraries could be
# loaded, this is an unavoidable problem with the traditional shared library
# concept.
# Generating position-independent code is often the default behavior for most
# modern compilers.
# Moreover linking a static library that is not built with PIC from a shared
# library will fail on some compiler/architecture combinations.
# Further details on PIC can be found here:
# https://eli.thegreenplace.net/2011/11/03/position-independent-code-pic-in-shared-libraries/
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


#-------Important
#Let the singleton to be unique
#-------Important
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  if (ENABLE_MSVC_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  add_compile_options(-bigobj)
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")
endif ()

if(WIN32)
    set(Common_LIBRARIES)
else()
    set(Common_LIBRARIES dl pthread)
endif()

# Enable RPATH support for installed binaries and libraries
include(AddInstallRPATHSupport)
add_install_rpath_support(BIN_DIRS "${CMAKE_INSTALL_FULL_BINDIR}"
                          LIB_DIRS "${CMAKE_INSTALL_FULL_LIBDIR}"
                          INSTALL_NAME_DIR "${CMAKE_INSTALL_FULL_LIBDIR}"
                          USE_LINK_PATH)

add_subdirectory(src)

if(WIN32)
    add_custom_target(copy_xml_in_build ALL)
    add_custom_command(TARGET copy_xml_in_build
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/$<CONFIG>/tests)
    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/$<CONFIG>/config.xml"
                  INPUT "${CMAKE_CURRENT_LIST_DIR}/config.xml")
else()
    file(COPY ${PROJECT_SOURCE_DIR}/tests
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
    file(COPY ${PROJECT_SOURCE_DIR}/gzworld
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
    file(COPY "${CMAKE_CURRENT_LIST_DIR}/config.xml"
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
endif()

# conf files
install(DIRECTORY tests DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY gzworld DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES config.xml DESTINATION ${CMAKE_INSTALL_BINDIR})

include(AddUninstallTarget)

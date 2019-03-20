#
# Copyright (C) 2015 Cybernetica
#
# Research/Commercial License Usage
# Licensees holding a valid Research License or Commercial License
# for the Software may use this file according to the written
# agreement between you and Cybernetica.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 3.0 as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL included in the
# packaging of this file.  Please review the following information to
# ensure the GNU General Public License version 3.0 requirements will be
# met: http://www.gnu.org/copyleft/gpl-3.0.html.
#
# For further information, please contact us at sharemind@cyber.ee.
#

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(SharemindPdkHeaders VERSION 0.4.0 LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCMakeHelpers 1.3 REQUIRED)


FIND_PACKAGE(SharemindCxxHeaders 0.7.0 REQUIRED)
FIND_PACKAGE(SharemindModuleApis 1.0.0 REQUIRED)


# Headers:
FILE(GLOB_RECURSE SharemindPdkHeaders_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
ADD_CUSTOM_TARGET("include_SharemindPdkHeaders_HEADERS_in_IDE"
                  SOURCES ${SharemindPdkHeaders_HEADERS})
INSTALL(FILES ${SharemindPdkHeaders_HEADERS}
        DESTINATION "include/sharemind"
        COMPONENT "dev")


# Install CMake files:
SharemindCreateCMakeFindFiles(
    INCLUDE_DIRS
        "${CMAKE_INSTALL_PREFIX}/include"
        ${SharemindCxxHeaders_INCLUDE_DIRS}
        ${SharemindModuleApis_INCLUDE_DIRS}
    DEFINITIONS
        ${SharemindCxxHeaders_DEFINITIONS}
)


# Packaging:
SharemindSetupPackaging()
SharemindAddComponentPackage("dev"
    NAME "libsharemind-pdkheaders-dev"
    DESCRIPTION "Sharemind PDK development headers"
    DEB_SECTION "libdevel"
    DEB_DEPENDS
        "libsharemind-cxxheaders-dev (>= 0.7.0)"
        "sharemind-module-apis-dev (>= 1.0.0)"
        "libc6-dev (>= 2.19)"
)
SharemindPackagingFinalize()

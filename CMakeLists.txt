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
PROJECT(SharemindPdkHeaders VERSION 0.1.0 LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCMakeHelpers REQUIRED)

SharemindSetProjectVersion()


FIND_PACKAGE(SharemindCxxHeaders REQUIRED)
FIND_PACKAGE(SharemindLibModapi REQUIRED)


# Headers:
FILE(GLOB_RECURSE SharemindPdkHeaders_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
ADD_CUSTOM_TARGET("include_SharemindPdkHeaders_HEADERS_in_IDE"
                  SOURCES ${SharemindPdkHeaders_HEADERS})
INSTALL(FILES ${SharemindPdkHeaders_HEADERS}
        DESTINATION "include/sharemind"
        COMPONENT "dev")


# Install CMake files:
SharemindNewUniqueList(SharemindPdkHeaders_INSTALL_LINK_LIBRARIES)
SharemindNewUniqueList(SharemindPdkHeaders_INSTALL_INCLUDE_DIRS
    "${CMAKE_INSTALL_PREFIX}/include"
    ${SharemindCxxHeaders_INCLUDE_DIRS}
    ${SharemindLibModapi_INCLUDE_DIRS}
)
SharemindNewUniqueList(SharemindPdkHeaders_INSTALL_DEFINITIONS
    ${SharemindCxxHeaders_DEFINITIONS}
    ${SharemindLibModapi_DEFINITIONS}
)
SharemindUseCMakeFindFiles()


# Packaging:
SharemindSetupPackaging()
SharemindAddComponentPackage("dev"
    NAME "libsharemind-pdkheaders-dev"
    DESCRIPTION "Sharemind PDK development headers"
    DEB_SECTION "libdevel"
    DEB_DEPENDS
        "libsharemind-modapi-dev (>= 0.1.0)"
        "libsharemind-cxxheaders-dev (>= 0.1.0)"
        "libc6-dev (>= 2.19)"
)
SharemindPackagingFinalize()

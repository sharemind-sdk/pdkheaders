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
PROJECT(SharemindPdkHeaders VERSION 0.5.0 LANGUAGES CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

FIND_PACKAGE(SharemindCMakeHelpers 1.6 REQUIRED)


FIND_PACKAGE(SharemindCxxHeaders 0.8.0 REQUIRED)
FIND_PACKAGE(SharemindModuleApis 1.1.0 REQUIRED)


# Headers:
FILE(GLOB_RECURSE SharemindPdkHeaders_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
SharemindAddInterfaceLibrary(PdkHeaders
    EXPOSE_FILES ${SharemindPdkHeaders_HEADERS})
TARGET_INCLUDE_DIRECTORIES(PdkHeaders
    INTERFACE
        # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src> # TODO
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)
TARGET_LINK_LIBRARIES(PdkHeaders
    INTERFACE
        Sharemind::CxxHeaders
        Sharemind::ModuleApis
)
INSTALL(FILES ${SharemindPdkHeaders_HEADERS}
        DESTINATION "include/sharemind"
        COMPONENT "dev")
SharemindCreateCMakeFindFilesForTarget(PdkHeaders
    DEPENDENCIES
        "SharemindCxxHeaders 0.8.0"
        "SharemindModuleApis 1.1.0"
)


# Packaging:
SharemindSetupPackaging()
SharemindAddComponentPackage("dev"
    NAME "libsharemind-pdkheaders-dev"
    DESCRIPTION "Sharemind PDK development headers"
    DEB_SECTION "libdevel"
    DEB_DEPENDS
        "libsharemind-cxxheaders-dev (>= 0.8.0)"
        "sharemind-module-apis-dev (>= 1.1.0)"
        "libc6-dev (>= 2.19)"
)
SharemindPackagingFinalize()

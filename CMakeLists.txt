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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(SharemindPdkHeaders CXX)

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.local" OPTIONAL)
INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.local" OPTIONAL)

SET(SHAREMIND_PDKHEADERS_VERSION "0.1.0")

FIND_PACKAGE(SharemindLibmodapi REQUIRED)
FIND_PACKAGE(Hairball 0.2.0 REQUIRED)

MACRO(SetList name)
  SET("${name}" "")
  LIST(APPEND "${name}" ${ARGN})
ENDMACRO()

SetList(SHAREMIND_PDKHEADERS_EXTERNAL_LINK_LIBRARIES)
SetList(SHAREMIND_PDKHEADERS_EXTERNAL_INCLUDE_DIRS
    ${Hairball_INCLUDE_DIRS}
    ${SharemindLibmodapi_INCLUDE_DIRS})
SetList(SHAREMIND_PDKHEADERS_EXTERNAL_DEFINITIONS
    ${Hairball_DEFINITIONS}
    ${SharemindLibmodapi_DEFINITIONS})
LIST(REMOVE_DUPLICATES SHAREMIND_PDKHEADERS_EXTERNAL_LINK_LIBRARIES)
LIST(REMOVE_DUPLICATES SHAREMIND_PDKHEADERS_EXTERNAL_INCLUDE_DIRS)
LIST(REMOVE_DUPLICATES SHAREMIND_PDKHEADERS_EXTERNAL_DEFINITIONS)

SetList(SHAREMIND_PDKHEADERS_INSTALL_LINK_LIBRARIES
        ${SHAREMIND_PDKHEADERS_EXTERNAL_LINK_LIBRARIES})
SET(SHAREMIND_PDKHEADERS_INSTALL_INCLUDE_DIRS
    "${CMAKE_INSTALL_PREFIX}/include"
    ${SHAREMIND_PDKHEADERS_EXTERNAL_INCLUDE_DIRS})
SetList(SHAREMIND_PDKHEADERS_INSTALL_DEFINITIONS
        ${SHAREMIND_PDKHEADERS_EXTERNAL_DEFINITIONS})
LIST(REMOVE_DUPLICATES SHAREMIND_PDKHEADERS_INSTALL_INCLUDE_DIRS)

FILE(GLOB_RECURSE SHAREMIND_PDKHEADERS_HEADERS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
ADD_CUSTOM_TARGET("include_SHAREMIND_PDKHEADERS_HEADERS_in_IDE"
                  SOURCES ${SHAREMIND_PDKHEADERS_HEADERS})

INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
        DESTINATION "include/sharemind")

FILE(GLOB_RECURSE SHAREMIND_PDKHEADERS_INFILES
     "${CMAKE_CURRENT_SOURCE_DIR}/*.in")
ADD_CUSTOM_TARGET("include_SHAREMIND_PDKHEADERS_INFILES_in_IDE"
                  SOURCES ${SHAREMIND_PDKHEADERS_INFILES})

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/SharemindPdkHeadersConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/SharemindPdkHeadersConfig.cmake"
  @ONLY)
INSTALL(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/SharemindPdkHeadersConfig.cmake"
  DESTINATION "lib/SharemindPdkHeaders")
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/SharemindPdkHeadersConfigVersion.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/SharemindPdkHeadersConfigVersion.cmake"
  @ONLY)
INSTALL(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/SharemindPdkHeadersConfigVersion.cmake"
  DESTINATION "lib/SharemindPdkHeaders")


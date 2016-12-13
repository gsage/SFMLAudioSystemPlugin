# - Try to find Gsage
# Once done this will define
#
#  GSAGE_FOUND - system has Gsage 
#  GSAGE_INCLUDE_DIRS - the Gsage include directory
#  GSAGE_LIBRARIES - Link these to use Gsage
#
#  Copyright (c) 2016 Artem Chernyshev <artem.0xD2@gmail.com>
#
# Redistribution and use is allowed according to the terms of the MIT license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

set(FIND_GSAGE_PATHS
  ${GSAGE_ROOT}
  $ENV{GSAGE_ROOT}
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt)

if (GSAGE_LIBRARIES AND GSAGE_INCLUDE_DIRS)
  # in cache already
  set(GSAGE_FOUND TRUE)
else (GSAGE_LIBRARIES AND GSAGE_INCLUDE_DIRS)
  if (NOT WIN32)
   include(FindPkgConfig)
   pkg_check_modules(GSAGE gsage)
  endif (NOT WIN32)

  if (GSAGE_FOUND)
    set(GSAGE_INCLUDE_DIRS
      ${GSAGE_INCLUDE_DIRS}
    )
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      set(GSAGE_LIBRARIES "${GSAGE_LIBRARY_DIRS}/lib${GSAGE_LIBRARIES}.dylib")
    else (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      set(GSAGE_LIBRARIES
        ${GSAGE_LIBRARIES}
      )
    endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(GSAGE_FOUND TRUE)
  else (GSAGE_FOUND)
    find_path(GSAGE_INCLUDE_DIR
      NAMES
        GsageFacade.h
      PATH_SUFFIXES include include/gsage
      PATHS
        ${FIND_GSAGE_PATHS}
    )

    find_library(GSAGE_CORE_LIBRARY
      NAMES
        GsageCore
      PATH_SUFFIXES lib
      PATHS
        ${FIND_GSAGE_PATHS}
    )

    find_path(GSAGE_LIBRARY_DIR
      NAMES
        GsageCore
      PATH_SUFFIXES lib
      PATHS
        ${FIND_GSAGE_PATHS}
    )

    find_path(GSAGE_CMAKE_MODULES
      NAMES
        Functions.cmake
      PATH_SUFFIXES share/gsage/cmake
      PATHS
        ${FIND_GSAGE_PATHS}
    )

    set(GSAGE_INCLUDE_DIRS
      ${GSAGE_INCLUDE_DIR}
    )
    set(GSAGE_LIBRARIES
      ${GSAGE_CORE_LIBRARY}
      ${GSAGE_LIBRARY}
    )

    set(GSAGE_LIBRARY_DIRS
      ${GSAGE_LIBRARY_DIR}
    )

    if (GSAGE_INCLUDE_DIRS AND GSAGE_LIBRARIES)
       set(GSAGE_FOUND TRUE)
    endif (GSAGE_INCLUDE_DIRS AND GSAGE_LIBRARIES)

    if (GSAGE_FOUND)
      if (NOT GSAGE_FIND_QUIETLY)
        message(STATUS "Found Gsage: ${GSAGE_LIBRARIES}")
      endif (NOT GSAGE_FIND_QUIETLY)
    else (GSAGE_FOUND)
      if (GSAGE_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find Gsage")
      endif (GSAGE_FIND_REQUIRED)
    endif (GSAGE_FOUND)
  endif (GSAGE_FOUND)


  # show the GSAGE_INCLUDE_DIRS and GSAGE_LIBRARIES variables only in the advanced view
  mark_as_advanced(GSAGE_INCLUDE_DIRS GSAGE_LIBRARIES)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${GSAGE_CMAKE_MODULES}/Modules/")
  include(${GSAGE_CMAKE_MODULES}/Functions.cmake)

endif (GSAGE_LIBRARIES AND GSAGE_INCLUDE_DIRS)

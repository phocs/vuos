# Install script for directory: /home/alessio/Desktop/vuos/vunet_modules

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetnull.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetnull.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetnull.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/vu/modules/vunetnull.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/vu/modules" TYPE SHARED_LIBRARY FILES "/home/alessio/Desktop/vuos/lib/vunetnull.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetnull.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetnull.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetnull.so")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetreal.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetreal.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetreal.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/vu/modules/vunetreal.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/vu/modules" TYPE SHARED_LIBRARY FILES "/home/alessio/Desktop/vuos/lib/vunetreal.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetreal.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetreal.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetreal.so")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetvdestack.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetvdestack.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetvdestack.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/vu/modules/vunetvdestack.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/vu/modules" TYPE SHARED_LIBRARY FILES "/home/alessio/Desktop/vuos/lib/vunetvdestack.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetvdestack.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetvdestack.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/vu/modules/vunetvdestack.so")
    endif()
  endif()
endif()


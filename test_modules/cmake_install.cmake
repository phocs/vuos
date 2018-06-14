# Install script for directory: /home/alessio/Desktop/vuos/test_modules

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
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/mountreal.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/mountreal.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/vu/modules/mountreal.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/vu/modules/mountreal.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/vu/modules" TYPE SHARED_LIBRARY FILES "/home/alessio/Desktop/vuos/lib/mountreal.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/mountreal.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/mountreal.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/vu/modules/mountreal.so")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/unreal.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/unreal.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/vu/modules/unreal.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/vu/modules/unreal.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/vu/modules" TYPE SHARED_LIBRARY FILES "/home/alessio/Desktop/vuos/lib/unreal.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/unreal.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/unreal.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/vu/modules/unreal.so")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealinfofs.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealinfofs.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealinfofs.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/vu/modules/unrealinfofs.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/vu/modules" TYPE SHARED_LIBRARY FILES "/home/alessio/Desktop/vuos/lib/unrealinfofs.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealinfofs.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealinfofs.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealinfofs.so"
         OLD_RPATH "/home/alessio/Desktop/vuos/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealinfofs.so")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealsock.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealsock.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealsock.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/vu/modules/unrealsock.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/vu/modules" TYPE SHARED_LIBRARY FILES "/home/alessio/Desktop/vuos/lib/unrealsock.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealsock.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealsock.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealsock.so")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealuidgid.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealuidgid.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealuidgid.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/vu/modules/unrealuidgid.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/vu/modules" TYPE SHARED_LIBRARY FILES "/home/alessio/Desktop/vuos/lib/unrealuidgid.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealuidgid.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealuidgid.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/vu/modules/unrealuidgid.so")
    endif()
  endif()
endif()


# Install script for directory: /home/alessio/Desktop/vuos

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/alessio/Desktop/vuos/scripts/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/umvu/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/cmd/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/test_modules/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/libvumod/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/vufuse/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/vudev/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/vudev_modules/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/include/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/vunet/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/vufs/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/vubinfmt/cmake_install.cmake")
  include("/home/alessio/Desktop/vuos/vunet_modules/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/alessio/Desktop/vuos/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")

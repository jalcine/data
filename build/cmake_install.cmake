# Install script for directory: /home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}/usr/lib/libwntrdata.so.0.0.1"
      "$ENV{DESTDIR}/usr/lib/libwntrdata.so.0"
      "$ENV{DESTDIR}/usr/lib/libwntrdata.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/lib/libwntrdata.so.0.0.1;/usr/lib/libwntrdata.so.0;/usr/lib/libwntrdata.so")
FILE(INSTALL DESTINATION "/usr/lib" TYPE SHARED_LIBRARY FILES
    "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/libwntrdata.so.0.0.1"
    "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/libwntrdata.so.0"
    "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/libwntrdata.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}/usr/lib/libwntrdata.so.0.0.1"
      "$ENV{DESTDIR}/usr/lib/libwntrdata.so.0"
      "$ENV{DESTDIR}/usr/lib/libwntrdata.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}/usr/share/cmake/WntrData/WntrDataLibraryDepends.cmake")
    FILE(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}/usr/share/cmake/WntrData/WntrDataLibraryDepends.cmake"
         "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/CMakeFiles/Export/_usr/share/cmake/WntrData/WntrDataLibraryDepends.cmake")
    IF(EXPORT_FILE_CHANGED)
      FILE(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}/usr/share/cmake/WntrData/WntrDataLibraryDepends-*.cmake")
      IF(OLD_CONFIG_FILES)
        MESSAGE(STATUS "Old export file \"$ENV{DESTDIR}/usr/share/cmake/WntrData/WntrDataLibraryDepends.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        FILE(REMOVE ${OLD_CONFIG_FILES})
      ENDIF(OLD_CONFIG_FILES)
    ENDIF(EXPORT_FILE_CHANGED)
  ENDIF()
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/share/cmake/WntrData/WntrDataLibraryDepends.cmake")
FILE(INSTALL DESTINATION "/usr/share/cmake/WntrData" TYPE FILE FILES "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/CMakeFiles/Export/_usr/share/cmake/WntrData/WntrDataLibraryDepends.cmake")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
     "/usr/share/cmake/WntrData/WntrDataLibraryDepends-debug.cmake")
FILE(INSTALL DESTINATION "/usr/share/cmake/WntrData" TYPE FILE FILES "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/CMakeFiles/Export/_usr/share/cmake/WntrData/WntrDataLibraryDepends-debug.cmake")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/share/cmake/WntrData/WntrDataConfig.cmake")
FILE(INSTALL DESTINATION "/usr/share/cmake/WntrData" TYPE FILE FILES "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/InstallFiles/WntrDataConfig.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/share/cmake/WntrData/WntrDataConfigVersion.cmake")
FILE(INSTALL DESTINATION "/usr/share/cmake/WntrData" TYPE FILE FILES "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/InstallFiles/WntrDataConfigVersion.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/include/wntr/data/iterator.hpp;/usr/include/wntr/data/linguistics.hpp;/usr/include/wntr/data/ontology.hpp;/usr/include/wntr/data/md5.hpp;/usr/include/wntr/data/wntrdata.hpp;/usr/include/wntr/data/serializer.hpp;/usr/include/wntr/data/models.hpp;/usr/include/wntr/data/config.hpp")
FILE(INSTALL DESTINATION "/usr/include/wntr/data" TYPE FILE FILES
    "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/include/wntr/data/iterator.hpp"
    "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/include/wntr/data/linguistics.hpp"
    "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/include/wntr/data/ontology.hpp"
    "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/include/wntr/data/md5.hpp"
    "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/include/wntr/data/wntrdata.hpp"
    "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/include/wntr/data/serializer.hpp"
    "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/include/wntr/data/models.hpp"
    "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/config.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/share/cmake-2.8/Modules/LibFindMacros.cmake")
FILE(INSTALL DESTINATION "/usr/share/cmake-2.8/Modules" TYPE FILE FILES "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/cmake/Modules/LibFindMacros.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/lib/pkgconfig/wntrdata.pc")
FILE(INSTALL DESTINATION "/usr/lib/pkgconfig" TYPE FILE FILES "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/InstallFiles/wntrdata.pc")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/home/jacky/.wintermute/linguistics")
FILE(INSTALL DESTINATION "/home/jacky/.wintermute" TYPE DIRECTORY FILES "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/data/linguistics")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/home/jacky/.wintermute/ontology")
FILE(INSTALL DESTINATION "/home/jacky/.wintermute" TYPE DIRECTORY FILES "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/data/ontology")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/jacky/Documents/Development/Projects/Desktop/Wintermute/~wintermute-devel/database/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)

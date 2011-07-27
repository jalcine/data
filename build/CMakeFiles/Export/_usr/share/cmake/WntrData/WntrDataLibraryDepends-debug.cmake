#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
SET(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "wntrdata" for configuration "Debug"
SET_PROPERTY(TARGET wntrdata APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
SET_TARGET_PROPERTIES(wntrdata PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "/usr/lib/libpython2.7.so;/usr/lib/libQtXml.so;/usr/lib/libQtCore.so;soprano;QtCore;/usr/lib/libboost_filesystem-mt.so;/usr/lib/libboost_signals-mt.so;/usr/lib/libboost_serialization-mt.so;/usr/lib/libboost_python.so"
  IMPORTED_LOCATION_DEBUG "/usr/lib/libwntrdata.so.0.0.1"
  IMPORTED_SONAME_DEBUG "libwntrdata.so.0"
  )

# Commands beyond this point should not need to know the version.
SET(CMAKE_IMPORT_FILE_VERSION)

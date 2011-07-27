# - Config file for the WntrData package
# It defines the following variables
#  WNTRDATA_INCLUDE_DIRS - include directories for WntrData
#  WNTRDATA_LIBRARY_DIRS - library directories for WntrData (normally not used!)
#  WNTRDATA_LIBRARIES    - libraries to link against
 
# Tell the user project where to find our headers and libraries
set(WNTRDATA_INCLUDE_DIRS "/usr/include/wntr/data;/usr/include/python2.7;/usr/include/qt4;/usr/include")
set(WNTRDATA_LIBRARY_DIRS "/usr/lib")
 
# Our library dependencies (contains definitions for IMPORTED targets)
include("/usr/share/cmake/WntrData/WntrDataLibraryDepends.cmake")
 
# These are IMPORTED targets created by WntrDataLibraryDepends.cmake
set(WNTRDATA_LIBRARIES wntrdata)

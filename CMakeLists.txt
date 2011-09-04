project(WntrData)
cmake_minimum_required(VERSION 2.8)

set(WNTRDATA_MAJOR_VERSION 0)
set(WNTRDATA_MINOR_VERSION 1)
set(WNTRDATA_MICRO_VERSION 0)
set(WNTRDATA_VERSION
    ${WNTRDATA_MAJOR_VERSION}.${WNTRDATA_MINOR_VERSION}.${WNTRDATA_MICRO_VERSION})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${PROJECT_BINARY_DIR})

#
# Custom modules
#
list(APPEND CMAKE_MODULE_PATH cmake/Modules)

#
# Dependency packages
#
find_package(Boost 1.42 COMPONENTS
    signals
    python
    REQUIRED)

find_package(PythonLibs REQUIRED)

find_package(Qt4 4.4.3 COMPONENTS
    QtCore
    QtXml
    REQUIRED)

include("${QT_USE_FILE}")

find_package(PkgConfig REQUIRED)
pkg_search_module(Soprano REQUIRED soprano)
## update: The above should be removed and have the FindSoprano.cmake used instead at (http://websvn.kde.org/%2Acheckout%2A/trunk/KDE/kdelibs/cmake/modules/FindSoprano.cmake)


#
# Main variables
#
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Database")
set(PROJECT_LABEL "Wintermute Database")

set(WNTRDATA_LOCALE "en" CACHE STRING "The standard locale to use in Wintermute.")
set(WNTRDATA_LIB_INSTALL_DIR lib)
set(WNTRDATA_DATA_INSTALL_DIR share/wintermute CACHE PATH "The directory where Wintermute's data files will be installed.")

set(WNTRDATA_LING_DIR "ling" CACHE PATH "The name of the folder that contains the linguistics information. There should be no trailing or leading slashes.")
set(WNTRDATA_ONTO_DIR "onto" CACHE PATH "The name of the folder that contains the ontology information. There should be no trailing or leading slashes.")

set(WNTRDATA_INCLUDE_INSTALL_DIR "include/wntr/data")

set(WNTRDATA_INCLUDE_DIRS "${WNTRDATA_INCLUDE_INSTALL_DIR}"
    ${PYTHON_INCLUDE_DIR}
    ${QT_INCLUDE_DIRS}
    ${Soprano_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})
include_directories(${WNTRDATA_INCLUDE_DIRS})

set(WNTRDATA_CMAKE_DIR share/cmake/WntrData)
set(WNTRDATA_LIBRARIES 
    ${PYTHON_LIBRARIES}
    ${QT_LIBRARIES}
    ${Soprano_LIBRARIES}
    ${Boost_LIBRARIES})

#- Checking the build type
if(CMAKE_BUILD_TYPE EQUAL Debug)
    set(DEBUG yes)
endif()

#- Turning directory variables to absolute
#foreach(in DATA CONFIG)
#    set(p WNTRDATA_${in}_DIR)
#    if(NOT IS_ABSOLUTE "${${p}}")
#        set(${p} "${CMAKE_INSTALL_PREFIX}/${${p}}")
#    endif()
#endforeach()

#
# Configures
#

configure_file("${PROJECT_SOURCE_DIR}/cmake/WntrDataConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfig.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/cmake/WntrDataConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfigVersion.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
    "${PROJECT_BINARY_DIR}/config.hpp" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/wntrdata.pc.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/wntrdata.pc" @ONLY)

#-----------------------------------------------------------------------------
add_subdirectory(src)
#-----------------------------------------------------------------------------

#
# Installs
#
install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfig.cmake"
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfigVersion.cmake"
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/wntrdata.pc"
    DESTINATION "${WNTRDATA_LIB_INSTALL_DIR}/pkgconfig/")

install(DIRECTORY "data/${WNTRDATA_LING_DIR}"
    DESTINATION "${WNTRDATA_DATA_INSTALL_DIR}")

install(DIRECTORY "data/${WNTRDATA_ONTO_DIR}"
    DESTINATION "${WNTRDATA_DATA_INSTALL_DIR}")

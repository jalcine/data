project(WntrData)
cmake_minimum_required(VERSION 2.8)

set(WNTRDATA_MAJOR_VERSION "0")
set(WNTRDATA_MINOR_VERSION "01")
set(WNTRDATA_VERSION
    ${WNTRDATA_MAJOR_VERSION}.${WNTRDATA_MINOR_VERSION})

include(WinterDefaults)
include(WinterLibsDependencies)
include(WinterMacros)
include(FindPythonLibs)
include(FindPkgConfig)
include(FindQt4)
include(FindQJSON)

#
# Custom modules
#
list(APPEND CMAKE_MODULE_PATH "cmake/Modules")

#
# Dependency packages
#
find_package(Boost 1.40 COMPONENTS
    filesystem
    signals
    serialization
    python
    REQUIRED)

find_package(Qt4 4.4.3 COMPONENTS
    QtCore
    QtXml
    REQUIRED)

include("${QT_USE_FILE}") # Have to include this here so that the ${QT_*} directives include the proper components.

find_package(PkgConfig REQUIRED)
pkg_search_module(Soprano REQUIRED soprano)
## update: The above should be removed and have the FindSoprano.cmake used instead at (http://websvn.kde.org/%2Acheckout%2A/trunk/KDE/kdelibs/cmake/modules/FindSoprano.cmake)


#
# Main variables
#
set(VS_KEYWORD Qt4VSv1.0) # Visual Studio, huh? (Only to make working in Qt Creator a bit more natural :])
set(VS_SCC_PROJECTNAME "Wintermute Database")
set(WNTRDATA_LIB_DIR "${WINTER_LIB_INSTALL_DIR}")
set(WNTRDATA_DATA_DIR "${WINTER_DATA_INSTALL_DIR}")
set(WNTRDATA_INCLUDE_DIR "${WINTER_INCLUDE_INSTALL_DIR}/wntr/data")
set(WNTRDATA_INCLUDE_DIRS "${WNTRDATA_INCLUDE_DIR}"
    ${PYTHON_INCLUDE_DIR}
    ${QT_INCLUDE_DIR}
    ${Soprano_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS})
set(WNTRDATA_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/WntrData")
set(WNTRDATA_LIBRARIES "${PYTHON_LIBRARIES}"
    ${QT_LIBRARIES}
    ${Soprano_LIBRARIES}
    ${Boost_LIBRARIES})

#- Checking the build type
if(CMAKE_BUILD_TYPE EQUAL Debug)
    set(DEBUG yes)
endif()
#- Turning directory variables to absolute
foreach(in DATA CONFIG)
    set(p WNTRDATA_${in}_DIR)
    if(NOT IS_ABSOLUTE "${${p}}")
        set(${p} "${CMAKE_INSTALL_PREFIX}/${${p}}")
    endif()
endforeach()

#
# Includes
#
include_directories(
    "${PROJECT_SOURCE_DIR}/include/wntr/data"
    "${PROJECT_SOURCE_DIR}/include/wntr"
    ${WNTRDATA_INCLUDE_DIRS})

#
# Files
#
file(GLOB WNTRDATA_SHPP include/wntr/data/*.hpp)
file(GLOB WNTRDATA_SCPP src/*.cpp)
file(GLOB WNTRDATA_FILES data/*.*)
file(GLOB CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")

#
# Targets
#
add_library(
    wntrdata
    SHARED
    ${WNTRDATA_SHPP}
    ${WNTRDATA_SCPP})

#
# Properties
#
set_target_properties(wntrdata
    PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION})

#
# Linking
#
target_link_libraries(wntrdata
    ${WNTRDATA_LIBRARIES})

#
# Configures
#
configure_file("${PROJECT_SOURCE_DIR}/cmake/WntrDataConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfig.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/cmake/WntrDataConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfigVersion.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
    "${PROJECT_BINARY_DIR}/config.hpp")

configure_file("${PROJECT_SOURCE_DIR}/wntrdata.pc.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/wntrdata.pc" @ONLY)

#
# Installs
#
install(TARGETS wntrdata
    EXPORT WntrDataLibraryDepends
    LIBRARY DESTINATION "${WNTRDATA_LIB_DIR}")

install(EXPORT WntrDataLibraryDepends
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfig.cmake"
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfigVersion.cmake"
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES ${WNTRDATA_SHPP} "${PROJECT_BINARY_DIR}/config.hpp"
    DESTINATION "${WNTRDATA_INCLUDE_DIR}"
    COMPONENT dev)

install(FILES ${CMAKE_MODULES}
    DESTINATION "${CMAKE_MODULE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/wntrdata.pc"
    DESTINATION "${WINTER_LIB_INSTALL_DIR}/pkgconfig/")

install(FILES ${WNTRDATA_LINGUISTICS_FILES}
    DESTINATION "${WNTRDATA_DATA_DIR}")

install(DIRECTORY data/linguistics
    DESTINATION "${WNTRDATA_DATA_DIR}")

install(DIRECTORY data/ontology
    DESTINATION "${WNTRDATA_DATA_DIR}")

install(FILES ${WNTRDATA_FILES}
    DESTINATION "${WNTRDATA_DATA_DIR}")

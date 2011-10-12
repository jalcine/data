project(WntrData)
cmake_minimum_required(VERSION 2.8)

set(WINTERMUTE_IS_PLUGIN ON)
set(WNTRDATA_MAJOR_VERSION 0)
set(WNTRDATA_MINOR_VERSION 0)
set(WNTRDATA_MICRO_VERSION 1)
set(WNTRDATA_UUID "a9b6b020-f4f2-11e0-be50-0800200c9a66")
set(WNTRDATA_VERSION
    ${WNTRDATA_MAJOR_VERSION}.${WNTRDATA_MINOR_VERSION}${WNTRDATA_MICRO_VERSION})

## Custom modules
list(APPEND CMAKE_MODULE_PATH "cmake/Modules")

## Packages
find_package(Wintermute 0.01 REQUIRED)
find_package(PythonLibs 2.7 REQUIRED)
find_package(Boost 1.42 COMPONENTS
    python
    REQUIRED)
find_package(Qt4 4.4.3 COMPONENTS
    QtCore
    QtDBus
    QtXml
    REQUIRED)

include("${QT_USE_FILE}")
include(WintermuteMacros)
include(WintermuteDefaults)

find_package(PkgConfig REQUIRED)
pkg_search_module(SOPRANO REQUIRED soprano)
## update: The above should be removed and have the FindSoprano.cmake used instead at (http://websvn.kde.org/%2Acheckout%2A/trunk/KDE/kdelibs/cmake/modules/FindSoprano.cmake)
enable_doxygen()

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Database")
set(PROJECT_LABEL "Wintermute Database")
set(WNTRDATA_DATA_DIR "${WINTER_DATA_INSTALL_DIR}" CACHE PATH "The directory where Wintermute's data files will be installed.")
set(WNTRDATA_LING_DIR "ling" CACHE PATH "The name of the folder that contains the linguistics information. There should be no trailing or leading slashes.")
set(WNTRDATA_ONTO_DIR "onto" CACHE PATH "The name of the folder that contains the ontology information. There should be no trailing or leading slashes.")
set(WNTRDATA_INCLUDE_DIR "${WINTER_PLUGIN_INCLUDE_INSTALL_DIR}/data")
set(WNTRDATA_INCLUDE_DIRS "${WNTRDATA_INCLUDE_DIR}"
    ${PYTHON_INCLUDE_DIR}
    ${WINTERMUTE_INCLUDE_DIRS}
    ${QT_INCLUDE_DIR}
    ${SOPRANO_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS})
set(WNTRDATA_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/WntrData")
set(WNTRDATA_LIBRARIES "${PYTHON_LIBRARIES}"
    ${QT_LIBRARIES}
    ${SOPRANO_LIBRARIES}
    ${Boost_LIBRARIES})

include_directories(${WNTRDATA_INCLUDE_DIRS})

## Files
file(GLOB WNTRDATA_SHPP src/*.hpp)
file(GLOB WNTRDATA_SCPP src/*.cpp)
file(GLOB CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")
QT4_WRAP_CPP(WNTRDATA_SCPP "${WNTRDATA_SHPP}")

## Targets
add_library(wntrdata SHARED ${WNTRDATA_SCPP})

set_target_properties(wntrdata
    PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION})

target_link_libraries(wntrdata ${WNTRDATA_LIBRARIES})

## Configs

configure_file("${PROJECT_SOURCE_DIR}/cmake/WntrDataConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfig.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/cmake/WntrDataConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfigVersion.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
    "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/wntrdata.pc.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/wntrdata.pc" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/spec.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/wntrdata.spec" @ONLY)

## Installs
install(TARGETS wntrdata
    EXPORT WntrDataLibraryDepends
    LIBRARY DESTINATION "${WINTER_LIB_INSTALL_DIR}")

install(EXPORT WntrDataLibraryDepends
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfig.cmake"
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfigVersion.cmake"
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES ${WNTRDATA_SHPP}
    DESTINATION "${WNTRDATA_INCLUDE_DIR}"
    COMPONENT dev)

install(FILES ${CMAKE_MODULES}
    DESTINATION "${CMAKE_MODULE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/wntrdata.pc"
    DESTINATION "${WINTER_LIB_INSTALL_DIR}/pkgconfig/")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/wntrdata.pc"
    DESTINATION "${WINTER_PLUGINSPEC_INSTALL_DIR}/wntrdata.pc")

install(DIRECTORY "data/${WNTRDATA_LING_DIR}" "data/${WNTRDATA_ONTO_DIR}"
    DESTINATION "${WINTER_DATA_INSTALL_DIR}")

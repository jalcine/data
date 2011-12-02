project(WntrData)
cmake_minimum_required(VERSION 2.8)

set(WINTERMUTE_IS_PLUGIN ON)
set(WNTRDATA_SOURCE ON)
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
find_package(QJson REQUIRED)
find_package(PythonLibs 2.7 REQUIRED)
find_package(Boost 1.46 COMPONENTS
    python
    REQUIRED)
find_package(Qt4 COMPONENTS
    QtCore
    QtXml
    QtDBus
    QtGui
    REQUIRED)

include("${QT_USE_FILE}")
include(WintermuteMacros)
include(WintermuteDefaults)
enable_doxygen()

if(not CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

if(not CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX /usr)
endif()

find_package(PkgConfig REQUIRED)
pkg_search_module(SOPRANO REQUIRED soprano)
## update: The above should be removed and have the FindSoprano.cmake used instead at (http://websvn.kde.org/%2Acheckout%2A/trunk/KDE/kdelibs/cmake/modules/FindSoprano.cmake)

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Database")
set(PROJECT_LABEL "Wintermute Database")
set(WNTRDATA_DATA_DIR "${WINTER_PLUGIN_DATA_INSTALL_DIR}/data" CACHE PATH "The directory where Wintermute's data files will be installed.")
set(WNTRDATA_LING_DIR "ling" CACHE PATH "The name of the folder that contains the linguistics information. There should be no trailing or leading slashes.")
set(WNTRDATA_ONTO_DIR "onto" CACHE PATH "The name of the folder that contains the ontology information. There should be no trailing or leading slashes.")
set(WNTRDATA_INCLUDE_DIR "${WINTER_PLUGIN_INCLUDE_INSTALL_DIR}/data")
set(WNTRDATA_INCLUDE_DIRS "${WNTRDATA_INCLUDE_DIR}"
        ${PYTHON_INCLUDE_DIR}
        ${qjson_INCLUDE_DIR}
        ${WINTERMUTE_INCLUDE_DIRS}
        ${QT_INCLUDE_DIR}
        ${SOPRANO_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS})
set(WNTRDATA_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/WntrData")
set(WNTRDATA_LIBRARIES "${PYTHON_LIBRARIES}"
        ${QT_LIBRARIES}
        ${qjson_LIBRARIES}
        ${SOPRANO_LIBRARIES}
        ${WINTERMUTE_LIBRARIES}
        ${Boost_LIBRARIES})

include_directories(${WNTRDATA_INCLUDE_DIRS})
add_subdirectory(src)

## Configs

configure_file("${PROJECT_SOURCE_DIR}/cmake/WntrDataConfig.cmake.in"
		"${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfig.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/cmake/WntrDataConfigVersion.cmake.in"
		"${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfigVersion.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
		"${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/spec.in"
		"${PROJECT_BINARY_DIR}/InstallFiles/${WNTRDATA_UUID}.spec" @ONLY)

#dbus_add_activation_service(org.thesii.Wintermute.Data.service.in)

## Installs
install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfig.cmake"
              "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfigVersion.cmake"
        DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES ${CMAKE_MODULES}
        DESTINATION "${CMAKE_MODULE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/${WNTRDATA_UUID}.spec"
        DESTINATION "${WINTER_PLUGINSPEC_INSTALL_DIR}")

install(DIRECTORY "data/${WNTRDATA_LING_DIR}" "data/${WNTRDATA_ONTO_DIR}"
        DESTINATION "${WNTRDATA_DATA_DIR}")

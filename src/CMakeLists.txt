project(WntrDataLib)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Files
file(GLOB WNTRDATA_HEADERS *.hpp)
file(GLOB WNTRDATA_SOURCES *.cpp)
file(GLOB ontology_SUBDIR_H "ontology/*.hpp")
file(GLOB ontology_SUBDIR_S "ontology/*.cpp")
set(WNTRDATA_HEADERS ${WNTRDATA_HEADERS} ${ontology_SUBDIR_H})
set(WNTRDATA_SOURCES ${WNTRDATA_SOURCES} ${ontology_SUBDIR_S})
file(GLOB WNTRDATA_CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")
QT4_WRAP_CPP(WNTRDATA_SOURCES ${WNTRDATA_HEADERS})

include_directories("ontology")
## Targets
add_library(wntrdata SHARED ${WNTRDATA_SOURCES})

set_target_properties(wntrdata
    PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION})

target_link_libraries(wntrdata ${WNTRDATA_LIBRARIES})

install(TARGETS wntrdata
    EXPORT WntrDataLibraryDepends
    LIBRARY DESTINATION "${WINTER_PLUGIN_INSTALL_DIR}")

install(EXPORT WntrDataLibraryDepends
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES ${WNTRDATA_HEADERS}
    DESTINATION "${WNTRDATA_INCLUDE_DIR}"
    COMPONENT dev)

project(WntrDataLib)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Files
file(GLOB WNTRDATA_HEADERS "*.hpp"
                           "*/*.hpp")
file(GLOB WNTRDATA_SOURCES "*.cpp"
                           "*/*.cpp")
list(REMOVE_ITEM WNTRDATA_HEADERS "dbus.hpp")

set(WNTRDATA_HEADERS ${WNTRDATA_HEADERS})
set(WNTRDATA_SOURCES ${WNTRDATA_SOURCES})
file(GLOB WNTRDATA_CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")
QT4_WRAP_CPP(WNTRDATA_SOURCES ${WNTRDATA_HEADERS})


include_directories("ontology")
## Targets
add_library(wplugin-data SHARED ${WNTRDATA_SOURCES})

set_target_properties(wplugin-data
    PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION})

target_link_libraries(wplugin-data ${WNTRDATA_LIBRARIES})

install(TARGETS wplugin-data
    EXPORT WntrDataLibraryDepends
    LIBRARY DESTINATION "${WINTER_PLUGIN_INSTALL_DIR}")

install(EXPORT WntrDataLibraryDepends
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES ${WNTRDATA_HEADERS}
    DESTINATION "${WNTRDATA_INCLUDE_DIR}"
    COMPONENT dev)

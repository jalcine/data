project(WntrDataLib)
cmake_minimum_required(VERSION 2.8)

## Files
file(GLOB WNTRDATA_SHPP *.hpp)
file(GLOB WNTRDATA_SCPP *.cpp)
file(GLOB CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")
QT4_WRAP_CPP(WNTRDATA_SCPP "${WNTRDATA_SHPP}")

## Targets
add_library(wntrdata SHARED ${WNTRDATA_SCPP})

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

install(FILES ${WNTRDATA_SHPP}
    DESTINATION "${WNTRDATA_INCLUDE_DIR}"
    COMPONENT dev)

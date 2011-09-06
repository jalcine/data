project(WntrDataLib)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Source files
file(GLOB WNTRDATA_SOURCES "*.cpp")

# Moc files
file(GLOB WNTRDATA_MOC_FILES "*.hpp")
QT_WRAP_CPP(WNTRDATA_SOURCES ${WNTRDATA_MOC_FILES})

add_library(wntrdata SHARED
    ${WNTRDATA_SOURCES})

set_target_properties(wntrdata PROPERTIES
    VERSION 0.0.1
    SOVERSION 0)

target_link_libraries(wntrdata
    ${WNTRDATA_LIBRARIES})

list(APPEND WNTRDATA_LIBRARIES wntrdata)

install(TARGETS wntrdata
    EXPORT WntrDataLibraryDepends
    LIBRARY DESTINATION ${WNTRDATA_LIB_INSTALL_DIR})

install(EXPORT WntrDataLibraryDepends
    DESTINATION share/cmake/WntrData)


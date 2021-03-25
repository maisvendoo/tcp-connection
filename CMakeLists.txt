file(GLOB SOURCES
    include/*.h
    src/*.cpp
)

add_library(TcpConnection ${SOURCES})

target_include_directories(TcpConnection PUBLIC include)

set_target_properties( TcpConnection
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}../../lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}../../lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}../../bin"
)

target_link_libraries(TcpConnection Qt5::Network)

# xrXMLParser - tinyxml-based XML parser
set(XRXMLPARSER_SOURCES
    StdAfx.cpp
    tinyxml.cpp
    tinyxmlerror.cpp
    tinyxmlparser.cpp
    xrXMLParser.cpp
)

add_library(xrxmlparser STATIC ${XRXMLPARSER_SOURCES})
target_include_directories(xrxmlparser PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/xrXMLParser"
)
target_link_libraries(xrxmlparser PUBLIC xrcore)

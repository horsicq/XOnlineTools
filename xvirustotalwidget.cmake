include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED XFORMATS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../Formats/xformats.cmake)
    set(XVIRUSTOTALWIDGET_SOURCES ${XVIRUSTOTALWIDGET_SOURCES} ${XFORMATS_SOURCES})
endif()

set(XVIRUSTOTALWIDGET_SOURCES
    ${XVIRUSTOTALWIDGET_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xonlinetools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xonlinetools.h
    ${CMAKE_CURRENT_LIST_DIR}/xvirustotal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xvirustotal.h
    ${CMAKE_CURRENT_LIST_DIR}/xvirustotalwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xvirustotalwidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xvirustotalwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/xonlinetoolsoptionswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xonlinetoolsoptionswidget.h
    ${CMAKE_CURRENT_LIST_DIR}/xonlinetoolsoptionswidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogxvirustotal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogxvirustotal.h
    ${CMAKE_CURRENT_LIST_DIR}/dialogxvirustotal.ui
)

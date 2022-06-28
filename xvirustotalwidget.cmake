include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../Formats/xformats.cmake)

set(XVIRUSTOTALWIDGET_SOURCES
    ${XFORMATS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/xonlinetools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xvirustotal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xvirustotalwidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xvirustotalwidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/xvirustotaldialog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xvirustotaldialog.ui
    ${CMAKE_CURRENT_LIST_DIR}/xonlinetoolsoptionswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xonlinetoolsoptionswidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/xonlinetoolsdialogprocess.cpp
)

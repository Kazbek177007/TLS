cmake_minimum_required(VERSION 3.19)
project(TLS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)       # включает автоматическую генерацию moc_*.cpp
set(CMAKE_AUTORCC ON)       # включает автоматическую генерацию ресурсов
set(CMAKE_AUTOUIC ON)       # включает автоматическую генерацию из .ui

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network Xml)

qt_standard_project_setup()

qt_add_executable(TLS
    main.cpp
    widget.cpp
    widget.h
    widget.ui
    percent.h percent.cpp
    icustomfees.h
)

target_link_libraries(TLS
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
        Qt::Xml
)

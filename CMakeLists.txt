cmake_minimum_required(VERSION 3.16)

project(second_problem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(second_problem
    main.cpp
    BaseConverter.cpp
    BigFraction.cpp
    BigInteger.cpp

    BaseConverter.h
    BigFraction.h
    BigInteger.h
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
)

target_link_libraries(second_problem PRIVATE Qt6::Widgets)

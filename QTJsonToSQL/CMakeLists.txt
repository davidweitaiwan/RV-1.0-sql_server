cmake_minimum_required(VERSION 3.16)

project(QTJsonToSQL VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Sql)


add_executable(QTJsonToSQL
    main.cpp
    SensorsClass.cpp
    SensorsClass.h
)

target_link_libraries(QTJsonToSQL PRIVATE Qt5::Core Qt5::Sql) 


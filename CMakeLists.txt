cmake_minimum_required(VERSION 3.30)
project(Sem3Lab4)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-O3")

find_package(Qt6 COMPONENTS Core Gui Widgets Charts REQUIRED)

add_executable(Sem3Lab4 main.cpp
        UI/TextViewer.cpp
        Rows.cpp
        Pages.cpp
)

target_link_libraries(Sem3Lab4 PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Charts)
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Core DBus)
set(CMAKE_AUTOMOC ON)

set(SOURCE_FILES
    main.cpp
    config.cpp
    command.cpp
    runner.cpp
)

add_executable(dome_statistic ${SOURCE_FILES} ${COMMANDS_SOURCE_FILES})
target_include_directories(dome_statistic PRIVATE . ../common)
target_link_libraries(dome_statistic PRIVATE nlohmann_json::nlohmann_json)
set_property(TARGET dome_statistic PROPERTY CXX_STANDARD 17)

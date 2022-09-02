cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

find_package(Qt5 REQUIRED COMPONENTS Core DBus)
set(CMAKE_AUTOMOC ON)

find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)

find_package(SQLite3 REQUIRED)

find_package(spdlog REQUIRED)

set(SOURCE_FILES
    commands/command.cpp
    commands/cputemperature.cpp
    commands/ipcamera.cpp
    commands/parser.cpp
    commands/result.cpp
    commands/roomhumidity.cpp
    commands/roomtemperature.cpp
    commands/statistic.cpp
    config/statistic.cpp
    config/db.cpp
    config/dir.cpp
    config/file.cpp
    config/ipcamera.cpp
    config/telegram.cpp
    io/db.cpp
    io/dir.cpp
    io/io.cpp
    utils.cpp
)

add_library(dome SHARED ${SOURCE_FILES})
target_include_directories(dome PRIVATE . ../common)
target_link_libraries(dome PUBLIC Qt5::Core Qt5::DBus)
target_link_libraries(dome PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome PRIVATE SQLite::SQLite3)
target_link_libraries(dome PRIVATE spdlog::spdlog_header_only)
set_property(TARGET dome PROPERTY CXX_STANDARD 17)

install(TARGETS dome
        LIBRARY DESTINATION /lib
)

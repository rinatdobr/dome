cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome_lib VERSION 1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(mosquitto REQUIRED libmosquitto)

set(SOURCE_FILES
    config/core.cpp
    config/database.cpp
    config/endpoint.cpp
    config/ipcamera.cpp
    config/openweather.cpp
    config/source.cpp
    config/telegram.cpp
    db/writer.cpp
    message/command/command.cpp
    message/command/info.cpp
    message/command/ipcamera.cpp
    message/command/statistic.cpp
    message/request/get.cpp
    message/saver/dbsaver.cpp
    message/saver/filesaver.cpp
    mosquitto/mosq.cpp
    mosquitto/receiver.cpp
    mosquitto/sender.cpp
    mosquitto/topic.cpp
    utils/dir.cpp
    utils/file.cpp
    utils/threader.cpp
    utils/utils.cpp
    utils/validatable.cpp
    endpoint.cpp
)

add_library(dome_lib SHARED ${SOURCE_FILES})
target_include_directories(dome_lib PRIVATE .)
target_link_libraries(dome_lib PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_lib PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_lib PRIVATE SQLite::SQLite3)
target_link_libraries(dome_lib PRIVATE ${mosquitto_LIBRARIES})
set_property(TARGET dome_lib PROPERTY CXX_STANDARD 17)

install(TARGETS dome_lib
        LIBRARY DESTINATION /lib
)

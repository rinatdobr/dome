cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome_lib VERSION 1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(mosquitto REQUIRED libmosquitto)

set(SOURCE_FILES
    config/core.cpp
    config/file.cpp
    config/ipcamera.cpp
    config/provider.cpp
    config/telegram.cpp
    data/getter.cpp
    data/provider.cpp
    mosquitto/mosq.cpp
    mosquitto/reciever.cpp
    mosquitto/sender.cpp
    utils/dir.cpp
    threader.cpp
    utils.cpp
    validatable.cpp
)

add_library(dome_lib SHARED ${SOURCE_FILES})
target_include_directories(dome_lib PRIVATE .)
target_link_libraries(dome_lib PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_lib PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_lib PRIVATE ${mosquitto_LIBRARIES})
set_property(TARGET dome_lib PROPERTY CXX_STANDARD 17)

install(TARGETS dome_lib
        LIBRARY DESTINATION /lib
)

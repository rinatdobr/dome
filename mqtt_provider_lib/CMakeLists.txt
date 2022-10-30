cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome_mqtt_provider_lib VERSION 1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(mosquitto REQUIRED libmosquitto)

set(SOURCE_FILES
    config/config.cpp
    config/file.cpp
    core/provider.cpp
    core/sender.cpp
    utils.cpp
)

add_library(dome_mqtt_provider_lib SHARED ${SOURCE_FILES})
target_include_directories(dome_mqtt_provider_lib PRIVATE .)
target_link_libraries(dome_mqtt_provider_lib PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_mqtt_provider_lib PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_mqtt_provider_lib PRIVATE ${mosquitto_LIBRARIES})

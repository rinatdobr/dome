cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome_mqtt_provider_lib VERSION 1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(mosquitto REQUIRED libmosquitto)

set(SOURCE_FILES
    provider.cpp
    sender.cpp
)

add_library(dome_mqtt_provider_lib SHARED ${SOURCE_FILES})
target_include_directories(dome_mqtt_provider_lib PRIVATE .)
target_include_directories(dome_mqtt_provider_lib PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(dome_mqtt_provider_lib PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_mqtt_provider_lib PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_mqtt_provider_lib PRIVATE ${mosquitto_LIBRARIES})
target_link_libraries(dome_mqtt_provider_lib PRIVATE dome_lib)

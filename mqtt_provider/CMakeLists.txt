cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(mqtt_provider VERSION 1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/../3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)
find_package(pigpio PATHS ${CMAKE_SOURCE_DIR}/../3rdparty/pigpio/build/root/lib/cmake/pigpio 0.71 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(mosquitto REQUIRED libmosquitto)

set(SOURCE_FILES
    config/config.cpp
    config/file.cpp
    providers/dht22.cpp
    provider.cpp
    sender.cpp
    main.cpp
)

set(DHTXXD_SOURCE_FILES
    ../3rdparty/DHTXXD/DHTXXD.c
)
set_source_files_properties(${DHTXXD_SOURCE_FILES} PROPERTIES LANGUAGE CXX)

add_executable(mqtt_provider ${SOURCE_FILES} ${DHTXXD_SOURCE_FILES})
target_include_directories(mqtt_provider PRIVATE .)
target_include_directories(mqtt_provider PRIVATE ../3rdparty/DHTXXD)
target_link_libraries(mqtt_provider PRIVATE spdlog::spdlog_header_only)
target_link_libraries(mqtt_provider PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(mqtt_provider PRIVATE pigpio::pigpiod_if2)
target_link_libraries(mqtt_provider PRIVATE ${mosquitto_LIBRARIES})

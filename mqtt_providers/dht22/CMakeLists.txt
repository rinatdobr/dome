cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome_dht22_provider VERSION 1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)
find_package(pigpio PATHS ${CMAKE_SOURCE_DIR}/3rdparty/pigpio/build/root/lib/cmake/pigpio 0.71 REQUIRED)
find_package(PkgConfig REQUIRED)

set(SOURCE_FILES
    dht22.cpp
    main.cpp
)

set(DHTXXD_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/3rdparty/DHTXXD/DHTXXD.c
)
set_source_files_properties(${DHTXXD_SOURCE_FILES} PROPERTIES LANGUAGE CXX)

add_executable(dome_dht22_provider ${SOURCE_FILES} ${DHTXXD_SOURCE_FILES})
target_include_directories(dome_dht22_provider PRIVATE .)
target_include_directories(dome_dht22_provider PRIVATE ${CMAKE_SOURCE_DIR}/mqtt_provider_lib)
target_include_directories(dome_dht22_provider PRIVATE ${CMAKE_SOURCE_DIR}/3rdparty/DHTXXD)
target_link_libraries(dome_dht22_provider PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_dht22_provider PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_dht22_provider PRIVATE pigpio::pigpiod_if2)
target_link_libraries(dome_dht22_provider PRIVATE dome_mqtt_provider_lib)

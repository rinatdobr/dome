cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome_core VERSION 1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(mosquitto REQUIRED libmosquitto)

set(SOURCE_FILES
    main.cpp
)

add_executable(dome_core ${SOURCE_FILES})
target_include_directories(dome_core PRIVATE .)
target_include_directories(dome_core PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(dome_core PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_core PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_core PRIVATE ${mosquitto_LIBRARIES})
target_link_libraries(dome_core PRIVATE dome_lib)

install(TARGETS dome_core
        RUNTIME DESTINATION /usr/bin
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dome_core.service
        DESTINATION /usr/lib/systemd/user/
)

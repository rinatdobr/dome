cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome_openweather VERSION 1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)
find_package(cpr PATHS ${CMAKE_SOURCE_DIR}/3rdparty/cpr/build/root/lib/cmake 1.9.0 REQUIRED)

set(SOURCE_FILES
    openweather.cpp
    main.cpp
)

add_executable(dome_openweather ${SOURCE_FILES})
target_include_directories(dome_openweather PRIVATE .)
target_include_directories(dome_openweather PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(dome_openweather PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_openweather PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_openweather PRIVATE dome_lib)
target_link_libraries(dome_openweather PRIVATE cpr::cpr)
set_property(TARGET dome_openweather PROPERTY CXX_STANDARD 17)

install(TARGETS dome_openweather
        RUNTIME DESTINATION /usr/bin
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dome_ep_openweather.service
        DESTINATION /usr/lib/systemd/user/
)

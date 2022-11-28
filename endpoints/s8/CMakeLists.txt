cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome_s8 VERSION 1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)
find_package(PkgConfig REQUIRED)

set(SOURCE_FILES
    s8.cpp
    main.cpp
)

add_executable(dome_s8 ${SOURCE_FILES} ${DHTXXD_SOURCE_FILES})
target_include_directories(dome_s8 PRIVATE .)
target_include_directories(dome_s8 PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(dome_s8 PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_s8 PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_s8 PRIVATE dome_lib)

install(TARGETS dome_s8
        RUNTIME DESTINATION /usr/bin
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dome_ep_s8.service
        DESTINATION /usr/lib/systemd/user/
)

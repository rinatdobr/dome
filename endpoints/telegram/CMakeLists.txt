cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome_telegram VERSION 1.0 LANGUAGES CXX)

find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)
find_package(spdlog REQUIRED)
find_package(Td PATHS ${CMAKE_SOURCE_DIR}/3rdparty/td/build/root/lib/cmake 1.8.4 REQUIRED)

set(SOURCE_FILES
    replier.cpp
    tdclient.cpp
    main.cpp
)

add_executable(dome_telegram ${SOURCE_FILES})
target_include_directories(dome_telegram PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(dome_telegram PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_telegram PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_telegram PRIVATE dome_lib)
target_link_libraries(dome_telegram PRIVATE Td::TdStatic)
set_property(TARGET dome_telegram PROPERTY CXX_STANDARD 17)

install(TARGETS dome_telegram
        RUNTIME DESTINATION /usr/bin
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dome_telegram.service
        DESTINATION /usr/lib/systemd/user/
)

cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

find_package(Qt5 REQUIRED COMPONENTS Core DBus)
set(CMAKE_AUTOMOC ON)

find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)

find_package(spdlog REQUIRED)

find_package(Td PATHS ${CMAKE_SOURCE_DIR}/3rdparty/td/build/root/lib/cmake 1.8.4 REQUIRED)

set(SOURCE_FILES
    commandor.cpp
    config.cpp
    main.cpp
    tdclient.cpp
)

add_executable(dome_client ${SOURCE_FILES})
target_include_directories(dome_client PRIVATE . ../lib)
target_link_libraries(dome_client PRIVATE dome)
target_link_libraries(dome_client PUBLIC Qt5::Core Qt5::DBus)
target_link_libraries(dome_client PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_client PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_client PRIVATE Td::TdStatic)
set_property(TARGET dome_client PROPERTY CXX_STANDARD 17)

install(TARGETS dome_client
        RUNTIME DESTINATION /usr/bin
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dome_client.service
        DESTINATION /usr/lib/systemd/user/
)

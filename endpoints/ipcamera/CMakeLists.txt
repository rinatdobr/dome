cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome_ipcamera VERSION 1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)
find_package(PkgConfig REQUIRED)

set(SOURCE_FILES
    ipcamera.cpp
    main.cpp
)

add_executable(dome_ipcamera ${SOURCE_FILES})
target_include_directories(dome_ipcamera PRIVATE .)
target_include_directories(dome_ipcamera PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(dome_ipcamera PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_ipcamera PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_ipcamera PRIVATE dome_lib)

install(TARGETS dome_ipcamera
        RUNTIME DESTINATION /usr/bin
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dome_ep_ipcamera.service
        DESTINATION /usr/lib/systemd/user/
)

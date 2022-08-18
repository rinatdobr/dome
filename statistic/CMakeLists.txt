cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Core DBus)
set(CMAKE_AUTOMOC ON)

find_package(SQLite3 REQUIRED)

find_package(spdlog REQUIRED)

set(SOURCE_FILES
    main.cpp
    config.cpp
    runner.cpp
)

set(COMMON_SOURCE_FILES
    ../common/commands/command.cpp
    ../common/commands/cputemperature.cpp
    ../common/commands/parser.cpp
    ../common/commands/result.cpp
    ../common/commands/statistic.cpp
    ../common/config/command.cpp
    ../common/config/commands.cpp
    ../common/config/config.cpp
    ../common/config/db.cpp
    ../common/io/db.cpp
    ../common/io/io.cpp
)

add_executable(dome_statistic ${SOURCE_FILES} ${COMMON_SOURCE_FILES})
target_include_directories(dome_statistic PRIVATE . ../common)
target_link_libraries(dome_statistic PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_statistic PUBLIC
    Qt5::Core
    Qt5::DBus
)
target_link_libraries(dome_statistic PRIVATE SQLite::SQLite3)
target_link_libraries(dome_statistic PRIVATE spdlog::spdlog_header_only)
set_property(TARGET dome_statistic PROPERTY CXX_STANDARD 17)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dome_statistic.service
        DESTINATION /usr/lib/systemd/user/
)

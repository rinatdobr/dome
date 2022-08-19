cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

IF (NOT DEFINED APP_ID)
    SET(APP_ID "NO_APP_ID")
ENDIF()
ADD_DEFINITIONS(-DAPP_ID=${APP_ID})

IF (NOT DEFINED APP_HASH)
    SET(APP_HASH "NO_APP_HASH")
ENDIF()
ADD_DEFINITIONS(-DAPP_HASH=${APP_HASH})

IF (NOT DEFINED CHAT_ID)
    SET(CHAT_ID 0)
ENDIF()
ADD_DEFINITIONS(-DCHAT_ID=${CHAT_ID})

IF (NOT DEFINED LOGIN)
    SET(LOGIN 0)
ENDIF()
ADD_DEFINITIONS(-DLOGIN=${LOGIN})

IF (NOT DEFINED PASS)
    SET(PASS 0)
ENDIF()
ADD_DEFINITIONS(-DPASS=${PASS})

find_package(Qt5 REQUIRED COMPONENTS Core DBus)
set(CMAKE_AUTOMOC ON)

find_package(nlohmann_json PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json/build/root/share/cmake 3.11.1 REQUIRED)

find_package(spdlog REQUIRED)

find_package(Td PATHS ${CMAKE_SOURCE_DIR}/3rdparty/td/build/root/lib/cmake 1.8.4 REQUIRED)

set(SOURCE_FILES
    main.cpp
    tdclient.cpp
    commandor.cpp
)

add_executable(dome_client ${SOURCE_FILES})
target_include_directories(dome_client PRIVATE . ../lib)
target_link_libraries(dome_client PRIVATE dome)
target_link_libraries(dome_client PUBLIC Qt5::Core Qt5::DBus)
target_link_libraries(dome_client PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(dome_client PRIVATE spdlog::spdlog_header_only)
target_link_libraries(dome_client PRIVATE Td::TdStatic)
set_property(TARGET dome_client PROPERTY CXX_STANDARD 14)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dome_client.service
        DESTINATION /usr/lib/systemd/user/
)

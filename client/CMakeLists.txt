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

find_package(Td PATHS ${CMAKE_SOURCE_DIR}/3rdparty/td/build/root/lib/cmake 1.8.4 REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Core DBus)
set(CMAKE_AUTOMOC ON)

set(SOURCE_FILES
    main.cpp
    tdclient.cpp
    commandor.cpp
    command.cpp
    result.cpp
)

set(COMMANDS_SOURCE_FILES
    commands/systeminfo.cpp
)

add_executable(dome ${SOURCE_FILES} ${COMMANDS_SOURCE_FILES})
target_include_directories(dome PRIVATE . ../common)
target_link_libraries(dome PRIVATE Td::TdStatic)
target_link_libraries(dome PUBLIC
    Qt5::Core
    Qt5::DBus
)
set_property(TARGET dome PROPERTY CXX_STANDARD 14)

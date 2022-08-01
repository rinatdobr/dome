cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

IF (NOT DEFINED APP_ID)
    SET(APP_ID "NO_APP_ID")
ENDIF()
ADD_DEFINITIONS(-DAPP_ID=${APP_ID})

IF (NOT DEFINED APP_HASH)
    SET(APP_HASH "NO_APP_HASH")
ENDIF()
ADD_DEFINITIONS(-DAPP_HASH=${APP_HASH})

find_package(Td PATHS ${CMAKE_SOURCE_DIR}/3rdparty/td/build/root/lib/cmake 1.8.4 REQUIRED)

set(SOURCE_FILES
    main.cpp
    tdclient.cpp
)

add_executable(dome ${SOURCE_FILES})
target_link_libraries(dome PRIVATE Td::TdStatic)
set_property(TARGET dome PROPERTY CXX_STANDARD 14)

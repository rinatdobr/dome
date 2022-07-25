cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

find_package(Td PATHS ${CMAKE_SOURCE_DIR}/3rdparty/td/build/root/lib/cmake 1.8.4 REQUIRED)

set(SOURCE_FILES
    main.cpp
)

add_executable(dome ${SOURCE_FILES})
target_link_libraries(dome PRIVATE Td::TdStatic)
set_property(TARGET dome PROPERTY CXX_STANDARD 14)

cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome VERSION 1.0 LANGUAGES CXX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-psabi")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")

add_subdirectory(lib)
add_subdirectory(core)
add_subdirectory(endpoints)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/domed.service
        DESTINATION /usr/lib/systemd/user/
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dome.target
        DESTINATION /usr/lib/systemd/user/
)

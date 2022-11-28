cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome VERSION 1.0 LANGUAGES CXX)

add_subdirectory(lib)
add_subdirectory(core)
add_subdirectory(endpoints)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/domed.service
        DESTINATION /usr/lib/systemd/user/
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dome.target
        DESTINATION /usr/lib/systemd/user/
)

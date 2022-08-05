cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

find_package(Qt5 REQUIRED COMPONENTS Core DBus)
set(CMAKE_AUTOMOC ON)

set(SOURCE_FILES
    dome.cpp
    sensors.cpp
    main.cpp
)

add_executable(dome_service ${SOURCE_FILES})
target_include_directories(dome_service PRIVATE ../common)
target_link_libraries(dome_service PUBLIC
    Qt5::Core
    Qt5::DBus
)

install(TARGETS dome_service
        RUNTIME DESTINATION /usr/bin
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ru.dome.conf
        DESTINATION /usr/share/dbus-1/system.d
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ru.dome.service
        DESTINATION /usr/share/dbus-1/system-services/
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dome.service
        DESTINATION /usr/lib/systemd/system/
)

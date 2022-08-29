cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

find_package(Qt5 REQUIRED COMPONENTS Core DBus)
set(CMAKE_AUTOMOC ON)

find_package(pigpio PATHS ${CMAKE_SOURCE_DIR}/3rdparty/pigpio/build/root/lib/cmake/pigpio 0.71 REQUIRED)

find_package(spdlog REQUIRED)

set(SOURCE_FILES
    sources/ipcamera.cpp
    sources/sensors.cpp
    sources/system.cpp
    config.cpp
    dome.cpp
    main.cpp
)

set(DHTXXD_SOURCE_FILES
    ../3rdparty/DHTXXD/DHTXXD.c
)
set_source_files_properties(${DHTXXD_SOURCE_FILES} PROPERTIES LANGUAGE CXX)

add_executable(domed ${SOURCE_FILES} ${DHTXXD_SOURCE_FILES})
target_include_directories(domed PRIVATE . ../lib)
target_include_directories(domed PRIVATE ../common ../3rdparty/DHTXXD)
target_link_libraries(domed PRIVATE dome)
target_link_libraries(domed PUBLIC Qt5::Core Qt5::DBus)
target_link_libraries(domed PRIVATE pigpio::pigpiod_if2)
target_link_libraries(domed PRIVATE spdlog::spdlog_header_only)

install(TARGETS domed
        RUNTIME DESTINATION /usr/bin
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ru.dome.conf
        DESTINATION /usr/share/dbus-1/system.d
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/ru.dome.service
        DESTINATION /usr/share/dbus-1/system-services/
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/domed.service
        DESTINATION /usr/lib/systemd/system/
)

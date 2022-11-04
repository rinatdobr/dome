cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(dome VERSION 1.0 LANGUAGES CXX)

add_subdirectory(lib)
add_subdirectory(core)
add_subdirectory(mqtt_provider_lib)
add_subdirectory(mqtt_providers)

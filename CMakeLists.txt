cmake_minimum_required(VERSION 3.28)

project(Piung VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Link the MSVC runtime statically (and SFML's, below) so the Windows
# executable runs without the Visual C++ Redistributable installed.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# SFML 3 is downloaded and built as static libraries by default, so the game
# builds anywhere without installing SFML first. Distro packagers can turn
# this off to link against a system-wide SFML 3 instead.
option(PIUNG_USE_SYSTEM_SFML "Use an installed SFML 3 instead of downloading it" OFF)

if(PIUNG_USE_SYSTEM_SFML)
    find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio)
else()
    include(FetchContent)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    # Build FreeType, Ogg, Vorbis and FLAC into the binary too, so release
    # builds don't depend on the versions a given distro ships.
    set(SFML_USE_SYSTEM_DEPS OFF CACHE BOOL "" FORCE)
    set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "" FORCE)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
    FetchContent_MakeAvailable(SFML)
endif()

add_executable(piung
    src/main.cpp
    src/Game.cpp
    src/State.cpp
    src/GameState.cpp
    src/Ball.cpp
    src/Paddle.cpp
    src/PlayerPaddle.cpp
    src/EnemyPaddle.cpp
    src/Resources.cpp)

target_link_libraries(piung PRIVATE SFML::Graphics SFML::Audio)

if(MSVC)
    target_compile_options(piung PRIVATE /W4)
else()
    target_compile_options(piung PRIVATE -Wall -Wextra -Wpedantic)
endif()

# On Windows, open no console window in release builds.
if(WIN32)
    set_target_properties(piung PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
    target_link_libraries(piung PRIVATE SFML::Main)
endif()

# Keep the game's assets next to the executable so it runs straight from the
# build directory.
add_custom_command(TARGET piung POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:piung>/Resources)

install(TARGETS piung RUNTIME DESTINATION .)
install(DIRECTORY Resources DESTINATION .)
install(FILES LICENSE README.md DESTINATION .)

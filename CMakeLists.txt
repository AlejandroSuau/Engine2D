cmake_minimum_required(VERSION 3.14)

project(Engine2D VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(ENGINE2D_BUILD_DEMOS "Build Engine2D demo apps" ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

include(FetchContent)

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
set(GLAD_DIR "${THIRD_PARTY_DIR}/glad")
set(GLM_DIR "${THIRD_PARTY_DIR}/glm")

# ================= SDL2 & deps =================
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.3
)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.2
)
FetchContent_MakeAvailable(SDL2_image)

FetchContent_Declare(
    SDL2_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-2.8.0
)
set(SDL2MIXER_VENDORED ON)
set(SDL2MIXER_OPUS OFF)
FetchContent_MakeAvailable(SDL2_mixer)

FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.22.0
)
FetchContent_MakeAvailable(SDL2_ttf)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# ================= GLAD =================
add_library(glad STATIC
    ${GLAD_DIR}/src/glad.c
)

target_include_directories(glad PUBLIC
    ${GLAD_DIR}/include
)

# ================= Engine2D library =================
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB_RECURSE ENGINE2D_SOURCES ${SOURCE_DIR}/*.cpp)

add_library(Engine2D STATIC ${ENGINE2D_SOURCES})

target_include_directories(Engine2D PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${GLM_DIR}
)

target_link_libraries(Engine2D
    PUBLIC
    glad
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    nlohmann_json::nlohmann_json
)

if (WIN32)
    target_link_libraries(Engine2D PUBLIC SDL2::SDL2main opengl32)
endif()

if (APPLE)
    target_link_libraries(Engine2D PUBLIC
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreVideo"
        "-framework AudioToolbox"
        "-framework CoreAudio"
        "-framework AVFoundation"
        "-framework GameController"
        "-framework Metal"
        "-framework Foundation"
        "-framework ForceFeedback"
        "-framework CoreHaptics"
        "-framework Carbon"
        "-framework OpenGL"
    )
endif()

if (ENGINE2D_BUILD_DEMOS)
    add_subdirectory(demos)
endif()
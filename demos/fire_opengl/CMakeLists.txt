# Librería con la lógica del juego (para compilar rápido y poder testear)
add_library(FireOpenGLShowcaseCore STATIC
    game/src/Game.cpp
)

target_include_directories(FireOpenGLShowcaseCore
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/game/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/game/src
)

target_link_libraries(FireOpenGLShowcaseCore
    PUBLIC Engine2D
)

# Ejecutable muy delgado: sólo main.cpp
add_executable(FireOpenGLShowcase game/src/main.cpp)
target_link_libraries(FireOpenGLShowcase PRIVATE FireOpenGLShowcaseCore)

# Copia de assets al lado del ejecutable (o usa symlinks en dev)
add_custom_command(TARGET FireOpenGLShowcase POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:FireOpenGLShowcase>/assets
)

# DLLs de SDL en Windows
if (WIN32)
    add_custom_command(TARGET FireOpenGLShowcase POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE:SDL2::SDL2main>
            $<TARGET_FILE:SDL2_image::SDL2_image>
            $<TARGET_FILE:SDL2_mixer::SDL2_mixer>
            $<TARGET_FILE:SDL2_ttf::SDL2_ttf>
        $<TARGET_FILE_DIR:FireOpenGLShowcase>
    )
endif()

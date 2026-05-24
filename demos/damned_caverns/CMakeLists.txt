# Librería con la lógica del juego (para compilar rápido y poder testear)
add_library(DamnedCavernsShowcaseCore STATIC
    game/src/Game.cpp
    game/src/DamnedCaverns.cpp
    game/src/Grid.cpp
)

target_include_directories(DamnedCavernsShowcaseCore
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/game/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/game/src
)

target_link_libraries(DamnedCavernsShowcaseCore
    PUBLIC Engine2D
)

# Ejecutable muy delgado: sólo main.cpp
add_executable(DamnedCavernsShowcase game/src/main.cpp)
target_link_libraries(DamnedCavernsShowcase PRIVATE DamnedCavernsShowcaseCore)

# Copia SIEMPRE los assets al lado del ejecutable al compilar
add_custom_target(CopyDamnedCavernsAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets
    COMMENT "Copying DamnedCavernsShowcase assets"
)
# DLLs de SDL en Windows
if (WIN32)
    add_custom_command(TARGET DamnedCavernsShowcase POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE:SDL2::SDL2main>
            $<TARGET_FILE:SDL2_image::SDL2_image>
            $<TARGET_FILE:SDL2_mixer::SDL2_mixer>
            $<TARGET_FILE:SDL2_ttf::SDL2_ttf>
        $<TARGET_FILE_DIR:DamnedCavernsShowcase>
    )
endif()
# Librería con la lógica del juego (para compilar rápido y poder testear)
add_library(PetRescueSagaShowcaseCore STATIC
    game/src/Game.cpp
    game/src/Grid.cpp
    game/src/PetRescueSaga.cpp
)

target_include_directories(PetRescueSagaShowcaseCore
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/game/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/game/src
)

target_link_libraries(PetRescueSagaShowcaseCore
    PUBLIC Engine2D
)

add_executable(PetRescueSagaShowcase game/src/main.cpp
        game/include/Block.hpp)
target_link_libraries(PetRescueSagaShowcase PRIVATE PetRescueSagaShowcaseCore)

# Copia los assets siempre al lado real del ejecutable
add_custom_command(TARGET PetRescueSagaShowcase POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:PetRescueSagaShowcase>/assets"
    COMMENT "Copying PetRescueSagaShowcase assets"
)

# DLLs de SDL en Windows
if (WIN32)
    add_custom_command(TARGET PetRescueSagaShowcase POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE:SDL2::SDL2main>
            $<TARGET_FILE:SDL2_image::SDL2_image>
            $<TARGET_FILE:SDL2_mixer::SDL2_mixer>
            $<TARGET_FILE:SDL2_ttf::SDL2_ttf>
        $<TARGET_FILE_DIR:PetRescueSagaShowcase>
    )
endif()
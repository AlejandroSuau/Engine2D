# demos/CMakeLists.txt
cmake_minimum_required(VERSION 3.14)

function(add_engine2d_demo NAME)
    cmake_parse_arguments(ARG "" "DIR" "" ${ARGN})
    if (NOT ARG_DIR)
        message(FATAL_ERROR "add_engine2d_demo(${NAME}) requires DIR <path>")
    endif()

    add_executable(${NAME} ${ARG_DIR}/main.cpp)
    target_link_libraries(${NAME} PRIVATE Engine2D)

    # Incluir headers del engine por si el demo usa APIs públicas
    target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

    # Copia de assets a carpeta de build del demo
    add_custom_command(TARGET ${NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${NAME}>/assets)

    if (WIN32)
        add_custom_command(TARGET ${NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2::SDL2>
                $<TARGET_FILE:SDL2::SDL2main>
                $<TARGET_FILE:SDL2_image::SDL2_image>
                $<TARGET_FILE:SDL2_mixer::SDL2_mixer>
                $<TARGET_FILE:SDL2_ttf::SDL2_ttf>
            $<TARGET_FILE_DIR:${NAME}>)
    endif()
endfunction()

# Demos
add_engine2d_demo(Sandbox        DIR ${CMAKE_CURRENT_SOURCE_DIR}/sandbox)
# add_engine2d_demo(PathfindingViewer DIR ${CMAKE_CURRENT_SOURCE_DIR}/pathfinding_viewer)

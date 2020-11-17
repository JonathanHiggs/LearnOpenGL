set(SOURCE_FILES
    Breakout/main.cpp
)

set(CLASSES
    BallObject
    Collisions
    Game
    GameLevel
    GameObject
    Particle
    ParticleGenerator
    PostProcessor
    PowerUp
    ResourceManager
    Shader
    SpriteRenderer
    Texture
)

foreach(f ${CLASSES})
    list(APPEND SOURCE_FILES "Breakout/${f}.cpp")
    list(APPEND SOURCE_FILES "Breakout/${f}.hpp")
endforeach()

add_executable(Breakout ${SOURCE_FILES})


get_filename_component(IRRKLANG_LIB_DIR "Vendor/irrKlang/lib/Winx64-visualStudio" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(IRRKLANG_LIB "${IRRKLANG_LIB_DIR}/irrKlang.lib")

set(IRRKLANG_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/irrKlang/bin/winx64-visualStudio")
set(IRRKLANG_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/irrKlang/include")


target_include_directories(Breakout PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${IRRKLANG_INCLUDES}
)

target_link_libraries(Breakout PRIVATE
    ${OPENGL_LIBRARY}
    glfw
    Glad
    glm
    stb
    ${IRRKLANG_LIB}
)

add_custom_command(TARGET Breakout POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${IRRKLANG_DLL_DIR}"
        $<TARGET_FILE_DIR:Breakout>)
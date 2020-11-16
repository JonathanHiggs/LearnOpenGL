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


target_include_directories(Breakout PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)


target_link_libraries(Breakout
    ${OPENGL_LIBRARY}
    glfw
    Glad
    glm
    stb
)
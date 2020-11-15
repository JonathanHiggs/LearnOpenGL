set(SOURCE_FILES
    main.cpp
    Game.cpp
    ResourceManager.cpp
    Shader.cpp
    SpriteRenderer.cpp
    Texture.cpp
)

set(HEADER_FILES
    Game.hpp
    ResourceManager.hpp
    Shader.hpp
    SpriteRenderer.hpp
    Texture.hpp
)


set(SOURCE_FILES_FULL "")
foreach(f ${SOURCE_FILES})
    list(APPEND SOURCE_FILES_FULL "Breakout/${f}")
endforeach()

set(HEADER_FILES_FULL "")
foreach(f ${HEADER_FILES})
    list(APPEND HEADER_FILES_FULL "Breakout/${f}")
endforeach()


add_executable(Breakout ${SOURCE_FILES_FULL} ${HEADER_FILES_FULL})


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
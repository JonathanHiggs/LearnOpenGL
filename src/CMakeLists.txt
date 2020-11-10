SET(SOURCE_FILES
    main.cpp
)


add_executable(LearnOpenGL ${SOURCE_FILES})

target_link_libraries(LearnOpenGL
    ${OPENGL_LIBRARY}
    glfw
    Glad
    glm
)
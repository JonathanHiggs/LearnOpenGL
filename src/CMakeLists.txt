SET(SOURCE_FILES
    main.cpp
)


SET(HEADER_FILES
    Shader.hpp
)


add_executable(LearnOpenGL ${SOURCE_FILES} ${HEADER_FILES})

#set_target_properties(LearnOpenGL PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(LearnOpenGL
    ${OPENGL_LIBRARY}
    glfw
    Glad
    glm
)
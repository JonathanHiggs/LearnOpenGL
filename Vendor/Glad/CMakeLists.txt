set(SOURCE_FILES
    src/glad.cpp
)

set(HEADER_FILES
    include/glad/glad.h
    include/KHR/khrplatform.h
)

add_library(Glad STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Glad
    ${OPEGL_LIBRARY}
)
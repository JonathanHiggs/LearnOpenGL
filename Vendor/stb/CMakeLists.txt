set(SOURCE_FILES
    src/stb.cpp
)

set(HEADER_FILES
    include/stb/stb_image.h
)

add_library(stb STATIC ${SOURCE_FILES} ${HEADER_FILES})
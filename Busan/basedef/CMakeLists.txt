cmake_minimum_required(VERSION 3.10)

# C Shared Project
project(basedef VERSION 1.0 LANGUAGES C)

# Sources List for Project
set(
    SOURCES
    "src/global.c"
    "src/fun.c"
 "src/obj.c")

# Headers for library
set(
    HEADERS
    "include/Busan/basedef/const.h"
    "include/Busan/basedef/global.h"
    "include/Busan/basedef/train.h"
    "include/Busan/basedef/fun.h"
    "include/Busan/basedef/obj.h"
)

# Build lib set
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# include directory
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# install set
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# install(FILES ${HEADERS} DESTINATION include/ae2fLib/Container)
cmake_minimum_required(VERSION 3.10)

# C Shared Project
project(obj VERSION 1.0 LANGUAGES C)

# Sources List for Project
set(
    SOURCES
    "src/zombie.c" "src/citizen.c" "src/madongseok.c" "src/villain.c")

# Headers for library
set(
    HEADERS
    "include/Busan/obj/zombie.h"
 "include/Busan/obj/citizen.h" "include/Busan/obj/madongseok.h" "include/Busan/obj/villain.h")

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

# dependencies
target_link_libraries(${PROJECT_NAME} PUBLIC 
    basedef
)

# install(FILES ${HEADERS} DESTINATION include/ae2fLib/Container)
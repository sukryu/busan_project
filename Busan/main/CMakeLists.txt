project(main VERSION 1.0 LANGUAGES C)

# CMakeList.txt: Busan에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#

# 이 프로젝트의 실행 파일에 소스를 추가합니다.
add_executable (main "src/main.c"    "src/initgame.c")

# dependencies
target_link_libraries(main PUBLIC 
    basedef
    obj
)

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
# include directory
target_include_directories(main
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
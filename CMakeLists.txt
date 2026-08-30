cmake_minimum_required(VERSION 3.16)

project(OOP
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Sources
file(GLOB TEST_SOURCES
    "test/*.cpp"
)

set(SOURCES
    main.cpp
    ${TEST_SOURCES}
)

add_executable(main
    ${SOURCES}
)

# Match Makefile release flags
target_compile_options(main PRIVATE
    -O2
)
cmake_minimum_required(VERSION 3.16)
project(JobSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(job_system
    main.cpp
    src/JobSystem.cpp
)

target_include_directories(job_system
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

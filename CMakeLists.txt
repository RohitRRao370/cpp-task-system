cmake_minimum_required(VERSION 3.16)
project(TaskSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task_system
    main.cpp
	executor/src/ThreadPoolExecutor.cpp
	task-system/benchmark/benchmark.cpp
	task-system/test/tests.cpp
)

target_include_directories(task_system
    PRIVATE
        ${PROJECT_SOURCE_DIR}/task-system/include
        ${PROJECT_SOURCE_DIR}/executor/include
		# ${PROJECT_SOURCE_DIR}/job-system/include
)

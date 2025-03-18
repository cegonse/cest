cmake_minimum_required(VERSION 3.5)
project(CestFramework)

# cest-runner
set(RUNNER_SOURCES
  ${PROJECT_SOURCE_DIR}/runner/helpers.cpp
  ${PROJECT_SOURCE_DIR}/runner/runner.cpp
  ${PROJECT_SOURCE_DIR}/runner/cmd-args.cpp
  ${PROJECT_SOURCE_DIR}/runner/watch-mode.cpp
)

SET(RUNNER_OS_SOURCES
  ${PROJECT_SOURCE_DIR}/runner/main.cpp
  ${PROJECT_SOURCE_DIR}/runner/directory.cpp
  ${PROJECT_SOURCE_DIR}/runner/output.cpp
  ${PROJECT_SOURCE_DIR}/runner/process.cpp
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(ASAN_CFLAGS "-DJSON_HAS_EXPERIMENTAL_FILESYSTEM=0")
  set(ASAN_LDFLAGS "")
else()
  set(ASAN_CFLAGS -fsanitize=address)
  set(ASAN_LDFLAGS -fsanitize=address -static-libasan)
endif()

SET(RUNNER_CFLAGS -g -O2 -Wall -Werror -std=c++20 ${ASAN_CFLAGS})
SET(TEST_CFLAGS -g -O0 -Wall -Werror -std=c++20 ${ASAN_CFLAGS})

add_executable(cest-runner ${RUNNER_SOURCES} ${RUNNER_OS_SOURCES})
target_compile_options(cest-runner PRIVATE ${RUNNER_CFLAGS})
target_link_options(cest-runner PRIVATE ${ASAN_LDFLAGS})

# runner tests
file(GLOB_RECURSE RUNNER_TESTS ${PROJECT_SOURCE_DIR}/runner/test/*.test.cpp)

foreach(TEST_FILE ${RUNNER_TESTS})
  get_filename_component(FILE_NAME ${TEST_FILE} NAME_WE)
  set(EXEC_NAME test_${FILE_NAME})
  add_executable(${EXEC_NAME} ${TEST_FILE} ${RUNNER_SOURCES} runner/test/helpers/helpers.cpp)
  target_include_directories(${EXEC_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/build)
  target_compile_options(${EXEC_NAME} PRIVATE ${TEST_CFLAGS})
  target_link_options(${EXEC_NAME} PRIVATE ${ASAN_LDFLAGS})
endforeach()

# cest tests
file(GLOB_RECURSE CEST_TESTS ${PROJECT_SOURCE_DIR}/test/*.test.cpp)

foreach(TEST_FILE ${CEST_TESTS})
  get_filename_component(FILE_NAME ${TEST_FILE} NAME_WE)
  set(EXEC_NAME test_${FILE_NAME})
  add_executable(${EXEC_NAME} ${TEST_FILE})
  target_include_directories(${EXEC_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/build)
  target_compile_options(${EXEC_NAME} PRIVATE -g ${TEST_CFLAGS})
  target_link_options(${EXEC_NAME} PRIVATE ${ASAN_LDFLAGS})
endforeach()

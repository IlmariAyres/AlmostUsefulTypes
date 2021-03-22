find_package(Catch2 REQUIRED)

set(TARGET_NAME "Tests")

set(CPP_TESTS
    main.cpp

    test_iterator.cpp
    test_stack.cpp
    test_tree.cpp
    )

add_executable(${TARGET_NAME} ${CPP_TESTS})
target_include_directories(${TARGET_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_custom_target(RUN_TESTS
    COMMAND ${TARGET_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    USES_TERMINAL
    )


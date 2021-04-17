add_subdirectory(gtest)

set(TEST_NAME ${CMAKE_PROJECT_NAME}_test)


set(TEST_SRC
        main.cpp
        test.cpp)

add_executable(${TEST_NAME} ${TEST_SRC})

include_directories(${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${TEST_NAME} ${PROJECT_NAME}_lib gtest_main gtest)
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})

add_subdirectory(gtest)

set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(TEST_SRC
        main.cpp
        board.cpp
        piece.cpp
        coord.cpp)

add_executable(${TEST_NAME} ${TEST_SRC})

target_link_libraries(${TEST_NAME}
        ${PROJECT_NAME}_lib
        gtest_main
        gtest)

if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif ()

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

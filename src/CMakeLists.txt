include_directories(${PROJECT_SOURCE_DIR}/include)

set(BUILD_SRC
        main.cpp
        )

add_executable(${PROJECT_NAME} ${BUILD_SRC})
add_library(${PROJECT_NAME}_lib STATIC ${BUILD_SRC})

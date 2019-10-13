cmake_minimum_required(VERSION 3.10)

project(spaceGame)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(${PROJECT_NAME}
    "application.cpp"
    "game.cpp"
    "main.cpp"
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME}
    ${PROJECT_NAME}::figure
    ${PROJECT_NAME}::primitive
    ${PROJECT_NAME}::scene
    ${PROJECT_NAME}::space
    CONAN_PKG::sdl2
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
)

enable_testing()

add_subdirectory(figure)
add_subdirectory(primitive)
add_subdirectory(scene)
add_subdirectory(space)

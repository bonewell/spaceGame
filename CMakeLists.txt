cmake_minimum_required(VERSION 3.10)

project(spaceGame)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(${PROJECT_NAME}
    "application.cpp"
    "figure/factory_shape.cpp"
    "figure/line.cpp"
    "figure/point.cpp"
    "figure/polygon.cpp"
    "figure/rectangle.cpp"
    "game.cpp"
    "main.cpp"
    "primitive/line.cpp"
    "primitive/point.cpp"
    "scene/sdl_scene.cpp"
    "scene/skeleton.cpp"
    "space/asteroid.cpp"
    "space/colorschema.cpp"
    "space/explosion.cpp"
    "space/nozzle.cpp"
    "space/projectile.cpp"
    "space/ship.cpp"
    "space/speed.cpp"
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME}
    CONAN_PKG::sdl2
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
)

enable_testing()

set(UNIT_TEST ${PROJECT_NAME}_unittest)
add_executable(${UNIT_TEST}
    "primitive/point.cpp"
    "primitive/point_test.cpp"
)
target_compile_features(${UNIT_TEST} PRIVATE cxx_std_17)
target_compile_options(${UNIT_TEST} PRIVATE  -fprofile-arcs -ftest-coverage)
target_link_libraries(${UNIT_TEST}
    CONAN_PKG::gtest
    -fprofile-arcs
    -ftest-coverage
)
add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})

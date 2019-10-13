set(MODULE_NAME primitive)
set(COMPONENT_NAME ${PROJECT_NAME}_${MODULE_NAME})

add_library(${COMPONENT_NAME}
    "line.cpp"
    "point.cpp"
)
add_library(${PROJECT_NAME}::${MODULE_NAME} ALIAS ${COMPONENT_NAME})
target_compile_features(${COMPONENT_NAME} PRIVATE cxx_std_17)
target_include_directories(${COMPONENT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}
)

set(UNIT_TEST ${COMPONENT_NAME}_unittest)
add_executable(${UNIT_TEST}
    "point_test.cpp"
)
target_compile_features(${UNIT_TEST} PRIVATE cxx_std_17)
target_compile_options(${UNIT_TEST} PRIVATE -fprofile-arcs -ftest-coverage)
target_link_libraries(${UNIT_TEST}
    ${COMPONENT_NAME}
    CONAN_PKG::gtest
    -fprofile-arcs
    -ftest-coverage
)
add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})

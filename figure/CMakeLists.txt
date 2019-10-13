set(MODULE_NAME figure)
set(COMPONENT_NAME ${PROJECT_NAME}_${MODULE_NAME})

add_library(${COMPONENT_NAME}
    "factory_shape.cpp"
    "line.cpp"
    "point.cpp"
    "polygon.cpp"
    "rectangle.cpp"
)
add_library(${PROJECT_NAME}::${MODULE_NAME} ALIAS ${COMPONENT_NAME})
target_compile_features(${COMPONENT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${COMPONENT_NAME}
    ${PROJECT_NAME}::primitive
    CONAN_PKG::sdl2
)

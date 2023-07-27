project(Parallels LANGUAGES CXX)
cmake_minimum_required(VERSION 3.22)
set(ANT_SOURCES Ants/ants_alg.cc Ants/graph.cc Ants/main.cc Ants/view.cc)
set(GAUSS_SOURCES gauss/model/gauss.cc gauss/view/view.cc gauss/main.cc)
set(WINOGRAD_SOURCES winograd/model/winograd.cc winograd/view/view.cc winograd/main.cc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra")

add_executable(ANTS ${ANT_SOURCES})
add_executable(GAUSS ${GAUSS_SOURCES})
add_executable(WINOGRAD ${WINOGRAD_SOURCES})

target_include_directories(ANTS PRIVATE Ants lib)
target_include_directories(GAUSS PRIVATE gauss lib gauss/model gauss/view)
target_include_directories(WINOGRAD PRIVATE winograd lib winograd/model winograd/view)
target_link_directories(ANTS PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(GAUSS PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(WINOGRAD PRIVATE ${CMAKE_SOURCE_DIR}/lib)


add_custom_target(build_ants
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ANTS
    DEPENDS ANTS)

add_custom_target(build_gauss
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target GAUSS
    DEPENDS GAUSS)

add_custom_target(build_winograd
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target WINOGRAD
    DEPENDS WINOGRAD)
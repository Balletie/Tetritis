file(GLOB SOURCES *.cpp)

add_executable(tetritis ${SOURCES})
target_link_libraries(tetritis sfml-window sfml-graphics sfml-system)
add_definitions(-std=c++11 -stdlib=libc++)
install(TARGETS tetritis RUNTIME DESTINATION ${CMAKE_SOURCE_DIR})


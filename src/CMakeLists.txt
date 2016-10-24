file(GLOB SOURCES *.cpp)

add_executable(tetritis ${SOURCES})
target_link_libraries(tetritis sfml-window sfml-graphics sfml-system tweeny)
set(CMAKE_CXX_STANDARD 14)
install(TARGETS tetritis RUNTIME DESTINATION ${CMAKE_SOURCE_DIR})


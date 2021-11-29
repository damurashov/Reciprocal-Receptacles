add_library(Rr INTERFACE)
target_include_directories(Rr INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Rr INTERFACE Options)

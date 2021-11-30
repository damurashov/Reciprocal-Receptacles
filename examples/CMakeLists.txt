file(GLOB PATH_EXAMPLES_SRC "*.cpp")

# Build all examples
add_library(ExamplesAll INTERFACE)

# List all *.cpp files
foreach(PATH_TARGET_SRC ${PATH_EXAMPLES_SRC})
	get_filename_component(NAME_TARGET_SRC ${PATH_TARGET_SRC} NAME_WE)
	string(CONCAT NAME_TARGET_SRC "Example" ${NAME_TARGET_SRC})
	add_executable(${NAME_TARGET_SRC} ${PATH_TARGET_SRC})
	target_link_libraries(${NAME_TARGET_SRC} Rr pthread)
	target_link_libraries(ExamplesAll INTERFACE Options)
	add_dependencies(ExamplesAll ${NAME_TARGET_SRC} Options)
endforeach()


add_library(Options INTERFACE)

macro(rr_option option_name option_description option_default)
	option(${option_name} ${option_description} ${option_default})
	target_compile_definitions(Options INTERFACE ${option_name}=${${option_name}})
endmacro()

# C++ standard library
rr_option(RRO_STL_ENABLE "Enable C++ STL standard library" ON)

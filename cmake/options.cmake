
add_library(Options INTERFACE)

macro(rr_bin_option option_name option_description option_default)
	option(${option_name} ${option_description} ${option_default})

	if(${option_name})
		target_compile_definitions(Options INTERFACE -D${option_name}=1)
	else()
		target_compile_definitions(Options INTERFACE -D${option_name}=0)
	endif()
endmacro()

# C++ standard library
rr_bin_option(RRO_STL_USED "C++ STL standard library is being used" ON)

# Safe callback conversion
rr_bin_option(RRO_STATIC_CAST_FN_CONVERSION "Use static_cast to convert callbacks. Requires deriving from base type" OFF)

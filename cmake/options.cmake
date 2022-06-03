
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

# Do use c assert() method
rr_bin_option(RRO_ENABLE_CASSERT "Use static_cast to convert callbacks. Requires deriving from base type" OFF)

# Enable debug output
rr_bin_option(RRO_ENABLE_DEBUG "Use debug output" ON)

# Whether the dynamic allocation capabilities are present
rr_bin_option(RRO_USE_NEW_OPERATOR "Provided environment has `new` operator defined. Some features rely on it." OFF)

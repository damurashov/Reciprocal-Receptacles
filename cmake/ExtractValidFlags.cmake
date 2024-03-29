# Convenience function that checks the availability of certain
# C or C++ compiler flags and returns valid ones as a string.

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

function(extract_valid_c_flags varname)
  set(valid_flags)
  foreach(flag IN LISTS ARGN)
    string(REGEX REPLACE "[^a-zA-Z0-9_]+" "_" flag_var ${flag})
    set(flag_var "C_FLAG_${flag_var}")
    check_c_compiler_flag("${flag}" "${flag_var}")
    if(${flag_var})
      set(valid_flags "${valid_flags} ${flag}")
    endif()
  endforeach()
  set(${varname} "${valid_flags}" PARENT_SCOPE)
endfunction()

function(extract_valid_cxx_flags varname)
  set(valid_flags)
  foreach(flag IN LISTS ARGN)
    string(REGEX REPLACE "[^a-zA-Z0-9_]+" "_" flag_var ${flag})
    set(flag_var "CXX_FLAG_${flag_var}")
    check_cxx_compiler_flag("${flag}" "${flag_var}")
    if(${flag_var})
      set(valid_flags "${valid_flags} ${flag}")
    endif()
  endforeach()
  set(${varname} "${valid_flags}" PARENT_SCOPE)
endfunction()

extract_valid_cxx_flags(RR_COMPILER_FLAGS
	"-Wall"
	"-pedantic"
	"-Wcast-qual"
	"-Wconversion"
	"-Wextra"
	"-Woverloaded-virtual"
	"-Wshadow"
	"-Wvla"
	"-Wuninitialized"
	"-Winit-self"
	"-Wdouble-promotion"
	"-Wstrict-aliasing"
	"-Wno-unused-local-typedefs"
	"-Winconsistent-missing-override"
	"-Wduplicated-cond"
	"-Wsuggest-override"
	"-Wno-psabi"
	"-Wno-stringop-truncation"
	"-Wno-unneeded-internal-declaration"
)
string(REPLACE " " ";" RR_COMPILER_FLAGS ${RR_COMPILER_FLAGS})

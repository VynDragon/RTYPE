add_library(module_sfml SHARED Module_SFML.cpp local.cpp)
if(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(module_sfml sfml-graphics sfml-window sfml-system)
endif(CMAKE_COMPILER_IS_GNUCXX)

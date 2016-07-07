add_library(engine STATIC Engine.cpp Worker.cpp bus.cpp)
if(CMAKE_COMPILER_IS_GNUCXX)
	target_link_libraries(engine dl pthread)
endif(CMAKE_COMPILER_IS_GNUCXX)

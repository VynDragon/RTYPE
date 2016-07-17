add_definitions(-DHAVE_STRUCT_TIMESPEC)
add_library(module_tick SHARED Module_Tick.cpp local.cpp)


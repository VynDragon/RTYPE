add_library(module_game SHARED Module_Game.cpp local.cpp)
add_library(module_game_client SHARED Module_Game.cpp local.cpp)
target_compile_definitions(module_game_client PUBLIC -DCLIENT)
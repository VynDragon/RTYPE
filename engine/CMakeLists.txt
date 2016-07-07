add_library(engine STATIC Engine.cpp Worker.cpp bus.cpp)
target_link_libraries(engine dl sfml-graphics sfml-window sfml-system pthread)

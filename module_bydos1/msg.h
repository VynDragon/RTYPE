#pragma once

// input
#define MSG_AI_SETNAME "aisetname" // std::string* name
// output
#define MSG_AI_MOVE "aimove" // std::tuple<std::string, float, float>* name, x, y
#define MSG_AI_SHOOT "aishoot" // std::string* name
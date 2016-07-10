#pragma once

// input
#define MSG_SFML_ADD_SPRITE "sfmladdsprite" // std::pair<std::string, std::string>*, string 1: identifier, string 2: sprite path
#define MSG_SFML_REMOVE_SPRITE "sfmlremovesprite" // take identifier std::string* as data
#define MSG_SFML_SETPOSITION_SPRITE "sfmlsetpositionsprite" // std::tuple<identifier, int, int>* ident, x, y
#define MSG_SFML_UPDATE "sfmlupdatewindow" // nullptr

//output

#define MSG_SFML_EVENT "sfmlevent" // sf::event*
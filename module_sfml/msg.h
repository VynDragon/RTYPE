#pragma once

// input
#define MSG_SFML_ADD_SPRITE "sfmladdsprite" // std::pair<std::string, std::string>*, string 1: identifier, string 2: sprite path
#define MSG_SFML_ADD_SPRITE_NETWORK "sfmladdspritenetwork" // char* of format identifier=spritepath0
#define MSG_SFML_REMOVE_SPRITE "sfmlremovesprite" // take identifier std::string* as data
#define MSG_SFML_SETPOSITION_SPRITE "sfmlsetpositionsprite" // std::tuple<identifier, float, float>* ident, x, y
#define MSG_SFML_SETPOSITION_SPRITE_NETWORK "sfmlsetpositionspritenetwork" // char* with identifier0rawfloatrawfloat as id x, y
#define MSG_SFML_SETCOLOR_SPRITE "sfmlsetcolorsprite" // std::tuple<identifier, uint8_t, uint8_t,uint8_t, uint8_t>* id r, g, b, a
#define MSG_SFML_SETCOLOR_SPRITE_NETWORK "sfmlsetcolorspritenetwork" // char* with identifier0rawuint8_trawuint8_trawuint8_trawuint8_t as id r, g, b, a
#define MSG_SFML_UPDATE "sfmlupdatewindow" // nullptr

//output

#define MSG_SFML_EVENT "sfmlevent" // sf::event*
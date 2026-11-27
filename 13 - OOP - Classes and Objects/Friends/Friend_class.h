#ifndef SECTION13_FRIENDS_FRIEND_CLASS_H
#define SECTION13_FRIENDS_FRIEND_CLASS_H

#include <string>
#include "Player.h"

class Friend_class {
public:
    void set_hero_name(Player &p, std::string name);
    void display_player(Player &p);
};

#endif  // SECTION13_FRIENDS_FRIEND_CLASS_H

//
// Created by tuo on 27.04.17.
//

#ifndef MYGAME_PLAYER1_H
#define MYGAME_PLAYER1_H

#include "cocos2d.h"
#include "Box2D/Box2D.h"

using namespace cocos2d;
class Player1 : public Sprite {
private:
    bool forward;
    int jet_power;
    int  live;

public:
    bool item;
    bool goal;

    void set_dest(bool t);
    bool get_dest();
    int get_live();

    int using_jet();
    int releasing_jet();
    bool is_jet_ready();
    int get_energy();
    void set_full_energy();

    int losing_live();
    bool is_alive();
    void set_full_live();

    Player1() = default;
    ~Player1() = default;
};


#endif //MYGAME_PLAYER1_H

//
// Created by tuo on 27.04.17.
//

#include <iostream>
#include "Player1.h"

void Player1::set_dest(bool t){
    forward = t;
}

bool Player1::get_dest(){
    return forward;
}

Player1::Player1():
    jet_power(100),
    goal(false),
    item(false),
    live(1)
{
}

bool Player1::is_jet_ready() {
    return jet_power > 0;
}

int Player1::using_jet(){
    if(jet_power > 0) {
        jet_power --;
    }
    return jet_power;
}

int Player1::releasing_jet(){
    if(jet_power < 100){
        if(jet_power == 0) {
            jet_power = 30;
        }
        jet_power ++;
    }
    return jet_power;
}

int Player1::losing_live() {
    if(live > 0) {
        return live--;
    }
    return live;
}

bool Player1::is_alive() {
    return live > 0;
}

void Player1::set_full_energy() {
    jet_power = 100;
}

void Player1::set_full_live() {
    live = 1;
}

int Player1::get_live() {
    return live;
}

int Player1::get_energy() {
    return jet_power;
}

//
// Created by tuo on 13.05.17.
//

#ifndef MYGAME_HISTORY_H
#define MYGAME_HISTORY_H
#include "cocos2d.h"

using namespace cocos2d;

class History: public Scene {
private:
    Label* info;
public:
    virtual bool init ();
    static Scene* createScene();
    void gotoNextLevel(float ct);
    CREATE_FUNC(History);
};
#endif //MYGAME_HISTORY_H

//
// Created by tuo on 09.05.17.
//

#ifndef MYGAME_LEVELCOMPLETESCENE_H
#define MYGAME_LEVELCOMPLETESCENE_H
#include "cocos2d.h"

using  namespace cocos2d;
class LevelCompleteScene: public Scene {
public:
    virtual bool init();
    static Scene* createScene();
    void gotoMenu(float ct);
    void gotoNextLevel(float ct);
    const int level_amount = 2;
    int level_number = 1;
    CREATE_FUNC(LevelCompleteScene);
};


#endif //MYGAME_LEVELCOMPLETESCENE_H

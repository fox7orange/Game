//
// Created by tuo on 09.05.17.
//

#ifndef MYGAME_MENU_H
#define MYGAME_MENU_H

#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class menu: public cocos2d::Scene {
public:
    virtual bool init();
    static Scene* createScene();


    CREATE_FUNC(menu);
private:

    Sprite* background;
    void menuCloseCallback(Ref* pSender);
    void gotoLevel(Ref* pSender);
};


#endif //MYGAME_MENU_H

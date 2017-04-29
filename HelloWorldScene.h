#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <chrono>

using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
private:
    TMXTiledMap* _tileMap;
    TMXLayer* _background;
    Sprite* _player;
    TMXLayer* _meta;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
    virtual void update(float delta) override;
    // implement the "static create()" method manually

    void setPlayerPosition(Point position);

    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__

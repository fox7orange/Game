#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Player1.h"
#include "Box2D/Box2D.h"
#include "level.h"
#include "History.h"

class HelloWorld : public level
{
private:
    level* lvl;
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__


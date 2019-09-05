//
// Created by tuo on 13.05.17.
//

#include <fstream>
#include "History.h"
#include "HelloWorldScene.h"

Scene* History::createScene()
{
    auto scene = Scene::create();
    auto layer = History::create();
    scene->addChild(layer);
    return  scene;
}

bool History::init() {
    if (!Scene::init()) {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("menu_background.png");
    background->setPosition(Point(origin.x+ visibleSize.width/4,
                                  origin.y + visibleSize.height - 33));
    addChild(background);

    info = Label::createWithSystemFont("history","history.ttf",13);
    info->setString("10/10/2150\nMain engineer of Tempest spaceship John Doe.\n"
                            "  Recieving abnormal signals from the cargo bay. Almost all the systems are down.\n"
                            "Communication with the rest of the crew is lost. I am locked in the engineering desk.\n"
                            "I am going to repair doors control.");
    info->setColor(Color3B(23,33,44));
    info->setPosition(Point(origin.x+ visibleSize.width/2,
                            origin.y + visibleSize.height/2));
    addChild(info, 1);

    int level_number = 0;

    std::ofstream file_writing("Resources/level_complete.txt");
    file_writing << (level_number + 1);
    file_writing.close();

    //this->scheduleOnce(schedule_selector(History::gotoNextLevel), 5);
    //CallFunc* start = CallFunc::create(CC_CALLBACK_0(History::gotoNextLevel, this));
    //this->runAction(Sequence::create(DelayTime::create(10), start, nullptr));

    this->scheduleOnce(schedule_selector(History::gotoNextLevel), 10);
    return true;
}

void History::gotoNextLevel(float ct) {
    auto l2 = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, l2));
}
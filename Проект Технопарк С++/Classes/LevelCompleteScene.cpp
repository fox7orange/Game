//
// Created by tuo on 09.05.17.
//

#include "LevelCompleteScene.h"
#include "menu.h"
#include <fstream>

Scene* LevelCompleteScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LevelCompleteScene::create();
    scene->addChild(layer);
    return  scene;
}

bool LevelCompleteScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("menu_background.png");
    background->setPosition(Point(origin.x+ visibleSize.width/4,
                                  origin.y + visibleSize.height - 33));
    addChild(background);

    auto info = Label::createWithSystemFont("hello","Arial.ttf",40);
    info->setString("Level complete");
    info->setPosition(Point(origin.x+ visibleSize.width/4 +  20,
                            origin.y + visibleSize.height - 33));
    addChild(info, 1);

    std::ifstream file_reading("Resources/level_complete.txt");
    file_reading >> level_number;
    file_reading.close();

    std::ofstream file_writing("Resources/level_complete.txt");
    file_writing << (level_number + 1);
    file_writing.close();

    if (level_number != level_amount)
        this->scheduleOnce(schedule_selector(LevelCompleteScene::gotoNextLevel), 4);
    else
        this->scheduleOnce(schedule_selector(LevelCompleteScene::gotoMenu), 4);
    return true;
}


void LevelCompleteScene::gotoMenu(float ct) {
    auto menu = menu::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(3, menu));
}

void LevelCompleteScene::gotoNextLevel(float ct) {
    auto l2 = HelloWorld::createScene();
    Director::getInstance()->replaceScene(l2);
}

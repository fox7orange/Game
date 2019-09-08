//
// Created by tuo on 09.05.17.
//

#include "menu.h"
#include "HelloWorldScene.h"
#include <fstream>
Scene* menu::createScene()
{
    auto scene = Scene::create();
    auto layer = menu::create();
    scene->addChild(layer);

    return  scene;
}

bool menu::init() {
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    background = Sprite::create("menu_background.png");
    background->setPosition(Point(visibleSize.width/2 +origin.x, visibleSize.height/2 + origin.y));
    this->addChild(background);

    Vector<MenuItem*> MenuItems;
    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(menu::menuCloseCallback, this));
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    MenuItems.pushBack(closeItem);

    auto playItem = MenuItemFont::create(
            "Play",
            CC_CALLBACK_1(menu::gotoLevel, this)
    );
    playItem->setPosition(Point(visibleSize.width/2 +origin.x, visibleSize.height/2 + origin.y));
    MenuItems.pushBack(playItem);

    auto menu = Menu::createWithArray(MenuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return  true;
}

void menu::gotoLevel(Ref* pSender) {
    auto lvl1 = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, lvl1));
}

void menu::menuCloseCallback(Ref* pSender) {
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
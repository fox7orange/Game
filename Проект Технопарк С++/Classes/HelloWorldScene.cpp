#include "HelloWorldScene.h"
#include <iostream>
#include <fstream>
#include "History.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return  HelloWorld::create();
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    std::ifstream file("Resources/level_complete.txt");
    if (!file.is_open()) {
        std::cout << "Файл не открыт!" << std::endl;
    }
    int num_l;
    file >> num_l;
    file.close();
    number_level = num_l;

    std::ifstream file1("Resources/history_complete.txt");
    if (!file1.is_open()) {
        std::cout << "Файл не открыт!" << std::endl;
    }
    int history_int;
    file1 >> history_int;
    file1.close();

    switch (num_l) {
        case 1: {
            lvl = creating_map("level1.tmx");

            if (history_int == 0) {
                auto background = Sprite::create("menu_background.png");
                background->setPosition(Point(origin.x + visibleSize.width / 4,
                                              origin.y + visibleSize.height - 33));
                addChild(background);

                auto info = Label::createWithSystemFont("history", "history.ttf", 15);
                info->setString("10/10/2150\nMain engineer of Tempest spaceship John Doe.\n"
                                        "  Recieving abnormal signals from the cargo bay. Almost all the systems are down.\n"
                                        "Communication with the rest of the crew is lost. I am locked in the engineering desk.\n"
                                        "I am going to repair doors control.");
                info->setPosition(Point(origin.x + visibleSize.width / 2,
                                        origin.y + visibleSize.height / 2));
                lvl->_label = info;
                lvl->addChild(_label, 1);
                auto eventListener = EventListenerKeyboard::create();
                eventListener->onKeyPressed = [] (EventKeyboard::KeyCode keyCode, Event* event) {
                    if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
                        std::ofstream file_writing("Resources/history_complete.txt");
                        file_writing <<  1;
                        file_writing.close();
                        auto lvl1 = HelloWorld::createScene();
                        Director::getInstance()->replaceScene(TransitionFade::create(1, lvl1));
                    }
                };
                this->_eventDispatcher->addEventListenerWithFixedPriority(eventListener, 1);
            }

            if (history_int == 1) {
                auto info = Label::createWithSystemFont("ello", "Arial.ttf", 13);
                info->setString("Radio signal::Can anybody hear me?Anybody?\n"
                                        "Engineer::Jane?\n"
                                        "Scientist::Oh, John, it`s you! I thought I was the only survivor.\n"
                                        "               Life support system is broken, we need to fix it");
                info->setColor(Color3B(0, 0, 0));
                info->setPosition(Point(origin.x + visibleSize.width / 4 + info->getContentSize().width / 4,
                                        origin.y + 30));
                lvl->_label = info;
                lvl->addChild(_label, 1);

                auto eventListener = EventListenerKeyboard::create();

                Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
                eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event *event) {
                    lvl->met(keyCode, event);
                    _label->setString("Engineer::Teleport is broken. Jane, can you fix it?\n"
                                              "Scientist::Ok, fortunately I`m locked near to teleport control.\n"
                                              "Engineer::Perfect. If so, I`ll try to find some tools. ");
                    if (lvl->player1->item) {
                        _label->setString("Engineer:: I found tools.");
                    }
                    if (lvl->player2->item) {
                        _label->setString("Scientist::Now I can fix teleport.");
                    }
                    if (lvl->player2->goal) {
                        _label->setString("Scientist:: John, I think the teleportation system is working.");
                    }
                };

                eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event *event) {
                    lvl->metRelease(keyCode, event);
                };

                this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
                this->scheduleUpdate();
            }
            break;
        }
        case 2: {
            lvl = creating_map("level2.tmx");
            lvl->removeChildByName("info1", true);
            auto info = Label::createWithSystemFont("hello","Arial.ttf",13);
            info->setString("Engineer::At least we won`t die because of lack of air. Any ideas?\n"
                                    "Scientist::I think we are locked at the engineering bay. Let`s find a way out.");
            info->setColor(Color3B(0, 0, 0));
            info->setPosition(Point(origin.x + visibleSize.width / 4 + info->getContentSize().width / 4,
                                    origin.y + 30));
            lvl->_label = info;
            lvl->addChild(_label, 1);

            auto eventListener = EventListenerKeyboard::create();

            Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
            eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
                lvl->met(keyCode,event);
                _label->setString("Engineer:: Oh shi... Bars are closed. Jane?\n"
                                          "Scientist::Urgh");
                if(lvl->player2->item)
                {
                    _label->setString("Scientist:: Going to fix the bars.");
                }
                if(lvl->player2->goal){
                    _label->setString("Scientist:: I have opened the bars.\nEngineer:: Let`s get out of here.");
                }
            };

            eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
                lvl->metRelease(keyCode, event);
            };

            this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener,this);
            this->scheduleUpdate();
            break;
        }
    }

    if (history_int != 0) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("back.mp3", true);
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);
    }

    return true;
}
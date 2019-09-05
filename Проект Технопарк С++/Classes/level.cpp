//
// Created by root on 04.05.17.
//

#include <iostream>
#include "level.h"
#include "menu.h"
#include "LevelCompleteScene.h"
#include <fstream>


static std::map <cocos2d::EventKeyboard::KeyCode,
        std::chrono::high_resolution_clock::time_point> keys;

void level::metRelease(EventKeyboard::KeyCode keyCode, Event* event){
    keys.erase(keyCode);
}

void  level::met(EventKeyboard::KeyCode keyCode, Event* event){
    if(keys.find(keyCode) == keys.end()){
        keys[keyCode] = std::chrono::high_resolution_clock::now();
    }
}

level* level::creating_map(char* mapname){
    //Добавление карты
    _tileMap = TMXTiledMap::create(mapname);

    _background = _tileMap->getLayer("background");
    _walls = _tileMap->getLayer("walls");

    //Определение недоступных областей карты
    _meta = _tileMap->getLayer("meta");
    _meta->setVisible(false);

    //собираемые объекты
    _collect = _tileMap->getLayer("collect");

    //цели
    _goals = _tileMap->getLayer("fix");

    //хз, как назвать, но от этого умереть можно, лол
    _warnings = _tileMap->getLayer("warnings");

    //энеогия, жизни, всякие полезности
    _resources = _tileMap->getLayer("energy");

    this->addChild(_tileMap);

    //spawn
    objectGroup = new  TMXObjectGroup();
    objectGroup = _tileMap->getObjectGroup("objects");
    creating_person(objectGroup);
    return this;
}

void level::creating_person(TMXObjectGroup* og) {
    auto spawnPoint1 = og->getObject("SpawnPoint1");
    int x = spawnPoint1.at("x").asInt();
    int y = spawnPoint1.at("y").asInt();

    player1 = new Player1();
    player1->initWithFile("engineer1_1.png");
    player1->setPosition(x,y);
    player1->setScale(0.02);
    player1->set_dest(true);
    this->addChild(player1);

    auto spawnPoint2 = og->getObject("SpawnPoint2");
    x = spawnPoint2.at("x").asInt();
    y = spawnPoint2.at("y").asInt();

    player2 = new Player1();
    player2->initWithFile("engineer2_1.png");
    player2->setPosition(x,y);
    player2->setScale(0.07);
    player2->set_dest(true);
    this->addChild(player2);

    indicators[0] = Sprite::create("indicator/indicator0.png");
    indicators[1] = Sprite::create("indicator/indicator1.png");
    indicators[2] = Sprite::create("indicator/indicator2.png");
    indicators[3] = Sprite::create("indicator/indicator3.png");
    indicators[4] = Sprite::create("indicator/indicator4.png");
    for (int i = 0; i < 5; i++) {
        indicators[i]->setScale(0.3);
        indicators[i]->setVisible(false);
        this->addChild(indicators[i]);
    }

    posForChange1 = player1->getPosition();
    posForChange2 = player2->getPosition();

    //добавление выхода из игры
    menu_staff();
}

void level::menu_staff(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(level::exit, this));
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 + 2,
                                origin.y + closeItem->getContentSize().height/2));
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}


void level::goals_compl() {
    if (player2->goal) {
        switch (number_level) {
            case 1:
                teleportation(player1);
                break;
            case 2:
                door_accept();
                break;
        };
    }
}

void level::teleportation(Player1* pl) {
    auto teleport_in = objectGroup->getObject("Teleport-in");
    if ((teleport_in.at("x").asFloat()+20 >= pl->getPosition().x) &&
            (teleport_in.at("x").asFloat()-20 <= pl->getPosition().x) &&
            (teleport_in.at("y").asFloat()+25  >= pl->getPosition().y) &&
            (teleport_in.at("y").asFloat()-25  <= pl->getPosition().y))
    {
        auto teleport_out = objectGroup->getObject("Teleport-out");
        pl->setPosition(teleport_out.at("x").asInt(), teleport_out.at("y").asInt() + 20);
    }
}

void level::door_accept() {
    for (int i = 0; i < 3; i++) {
        _meta->removeTileAt(coor_door[i]);
        _walls->removeTileAt(coor_door[i]);
    }
}

void level::update(float delta) {
    Player1 *player;

    if (player_active == 0) {
        player = player1;
        auto time1 = std::chrono::system_clock::now();
        while (1) {
            auto time2 = std::chrono::system_clock::now();
            std::vector<Point> Co_S = coordination_S(player2->getPosition());
            if (isValue(Co_S, player2)) {
                if (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() >= 15) {
                    auto moveBy = MoveBy::create(0.02, Point(0, -1));
                    player2->runAction(moveBy);
                    break;
                }
            } else {
                player2->releasing_jet();
                break;
            }
        }
    } else {
        player = player2;
        auto time1 = std::chrono::system_clock::now();
        while (1) {
            auto time2 = std::chrono::system_clock::now();
            std::vector<Point> Co_S = coordination_S(player1->getPosition());
            if (isValue(Co_S, player1)) {
                if (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() >= 15) {
                    auto moveBy = MoveBy::create(0.02, Point(0, -1));
                    player1->runAction(moveBy);
                    break;
                }
            } else {
                player1->releasing_jet();
                break;
            }
        }
    }

    Node::update(delta);

    if (number_level == 2)
        teleportation(player2);

    if (isKeyPressed(EventKeyboard::KeyCode::KEY_1)) {
        player_active = 0;
        goToP1();
    }
    if (isKeyPressed(EventKeyboard::KeyCode::KEY_2)) {
        player_active = 1;
        goToP2();
    }

    create_indicator(player);

    if (player1->goal) {
        win();
    }

    goals_compl();


    if (isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
        auto time1 = std::chrono::system_clock::now();
        if (!player->get_dest()) {
            player->setFlippedX(false);
        }
        player->set_dest(true);
        while (1) {
            auto time2 = std::chrono::system_clock::now();
            std::vector<Point> Co_D = coordination_D(player->getPosition());
            if (isValue(Co_D, player)) {
                if (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() >= 15) {
                    if (isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
                        if (player->is_jet_ready()) {
                            player->setPosition(Vec2(player->getPosition().x + 2, player->getPosition().y));
                        } else {
                            player->setPosition(Vec2(player->getPosition().x + 1, player->getPosition().y));
                        }
                        break;
                    } else {
                        std::vector<Point> Co_S = coordination_S(player->getPosition());
                        if (!isValue(Co_S, player)) {
                            change_imageAD();
                        }
                        player->setPosition(Vec2(player->getPosition().x + 1, player->getPosition().y));
                        break;
                    }
                }
            } else break;
        }
    }

    if (isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
        auto time1 = std::chrono::system_clock::now();
        if (player->get_dest()) {
            player->setFlippedX(true);
        }
        player->set_dest(false);
        while (1) {
            auto time2 = std::chrono::system_clock::now();
            std::vector<Point> Co_A = coordination_A(player->getPosition());
            if (isValue(Co_A, player)) {
                if (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() >= 15) {
                    if (isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
                        if (player->is_jet_ready()) {
                            player->setPosition(Vec2(player->getPosition().x - 2, player->getPosition().y));
                        } else {
                            player->setPosition(Vec2(player->getPosition().x - 1, player->getPosition().y));
                        }
                        break;
                    }
                    else {
                        std::vector<Point> Co_S = coordination_S(player->getPosition());
                        if (!isValue(Co_S, player)) {
                            change_imageAD();
                        }
                        player->setFlippedX(true);
                        player->setPosition(Vec2(player->getPosition().x - 1, player->getPosition().y));
                        break;
                    }
                }
            } else break;
        }
    }

    if (player->is_jet_ready()) {
        if (isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
            change_imageW();
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("jet.mp3", false);
            auto time1 = std::chrono::system_clock::now();
            while (1) {
                auto time2 = std::chrono::system_clock::now();
                std::vector <Point> Co_W = coordination_W(player->getPosition());
                if (isValue(Co_W, player)) {
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() >= 15) {
                        player->setPosition(Vec2(player->getPosition().x, player->getPosition().y + 1));
                        player->using_jet();
                        break;
                    }
                } else {
                    player->using_jet();
                    break;
                }
            }
        }
    } else {
        change_imageS();
        if (isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
            auto time1 = std::chrono::system_clock::now();
            while (1) {
                auto time2 = std::chrono::system_clock::now();
                std::vector<Point> Co_S = coordination_S(player->getPosition());
                if (isValue(Co_S, player)) {
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() >= 15) {
                        player->setPosition(Vec2(player->getPosition().x, player->getPosition().y - 1));
                        break;
                    }
                } else {
                    player->releasing_jet();
                    break;
                }
            }
        }
    }

    if (!isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
        auto time1 = std::chrono::system_clock::now();
        while (1) {
            auto time2 = std::chrono::system_clock::now();
            std::vector<Point> Co_S = coordination_S(player->getPosition());
            if (isValue(Co_S, player)) {
                change_imageS();
                if (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() >= 15) {
                    player->setPosition(Vec2(player->getPosition().x, player->getPosition().y - 1));
                    break;
                }
            } else {
                player->releasing_jet();
                break;
            }
        }
    }
}

bool level::isKeyPressed(EventKeyboard::KeyCode code) {
    if(keys.find(code) != keys.end()) {
        return true;
    }
    return false;
}

//Получаем координаты тайла куда будет двигаться герой
std::vector<Point> level::coordination_D(Point pos) {
    std::vector<Point> vec;
    int x = (pos.x  + 20) / _tileMap->getTileSize().width;
    int y1 = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y) / _tileMap->getTileSize().height;
    int y2 = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y - 20) / _tileMap->getTileSize().height;
    int y3 = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y + 20) / _tileMap->getTileSize().height;
    vec.push_back(Point(x,y1));
    vec.push_back(Point(x,y2));
    vec.push_back(Point(x,y3));
    return vec;
}

std::vector<Point> level::coordination_A(Point pos) {
    std::vector<Point> vec;
    int x = (pos.x - 20) / _tileMap->getTileSize().width;
    int y1 = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y) / _tileMap->getTileSize().height;
    int y2 = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y - 20) / _tileMap->getTileSize().height;
    int y3 = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y + 20) / _tileMap->getTileSize().height;
    vec.push_back(Point(x,y1));
    vec.push_back(Point(x,y2));
    vec.push_back(Point(x,y3));
    return vec;
}

std::vector<Point> level::coordination_W(Point pos) {
    std::vector<Point> vec;
    int x1 = pos.x / _tileMap->getTileSize().width;
    int x2 = (pos.x  - 15) / _tileMap->getTileSize().width;
    int x3 = (pos.x + 15) / _tileMap->getTileSize().width;
    int y = (((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y - 23) / _tileMap->getTileSize().height);
    vec.push_back(Point(x1,y));
    vec.push_back(Point(x2,y));
    vec.push_back(Point(x3,y));
    return vec;
}

std::vector<Point> level::coordination_S(Point pos) {
    std::vector<Point> vec;
    int x1 = pos.x / _tileMap->getTileSize().width;
    int x2 = (pos.x - 15) / _tileMap->getTileSize().width;
    int x3 = (pos.x + 15) / _tileMap->getTileSize().width;
    int y = (((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y + 23) / _tileMap->getTileSize().height);
    vec.push_back(Point(x1,y));
    vec.push_back(Point(x2,y));
    vec.push_back(Point(x3,y));
    return vec;
}

//Проверяем доступность тайла с вычисленными выше координатами
bool level::isValue(std::vector<Point> tileCoord, Player1 * player) {
    bool flag = false;
    if (_background) {
        if ((tileCoord[1].x > 0) && (tileCoord[1].y > 0) &&
            (tileCoord[2].x <= _background->getLayerSize().width) &&
            (tileCoord[1].y <= _background->getLayerSize().height)) {
            isWarning(tileCoord, player);
            isAchievening(tileCoord, player);
            Collecting(tileCoord, player);
            Collecting_res(tileCoord, player);
            flag = true;
        }
        if (!isMeta(tileCoord) && (flag)) {
            return true;
        }
    }
    return false;
}

void level::collecting_res(Point tileCoord, Player1* player){
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get.mp3", false);
    player->set_full_energy();
    player->set_full_live();
    _resources->removeTileAt(tileCoord);
}

void level::collecting_items(Point tileCoord, Player1* player){
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get.mp3", false);
    _collect->removeTileAt(tileCoord);
    player->item = true;
}

void level::achieving_goals(Point tileCoord, Player1* player) {
    if (player->item) {
        _goals->removeTileAt(tileCoord);
        Sprite* s = new Sprite;
        int x=0, y=0;
        if(player_active == 0){
            auto point = objectGroup->getObject("panel1");
            x = point.at("x").asInt();
            y = point.at("y").asInt();
        }
        else{
            auto point = objectGroup->getObject("panel2");
            x = point.at("x").asInt();
            y = point.at("y").asInt();
        }

        s->initWithFile("control_panel.png");
        s->setPosition(x,y);
        addChild(s);
        player->goal = true;
    }
}

void level::losing_lives(Player1* player) {
    player->losing_live();
    if (player == player1) {
        if (!player->is_alive()) {
            auto newScene = HelloWorld::createScene();
            Director::getInstance()->replaceScene(newScene);
        }
    } else {
        if (!player->is_alive()) {
            auto newScene = HelloWorld::createScene();
            Director::getInstance()->replaceScene(newScene);
        }
    }
}

bool level::isMeta(std::vector<Point> tileCoord) {
    Sprite* TileM1 = _meta->getTileAt(tileCoord[0]);
    Sprite* TileM2 = _meta->getTileAt(tileCoord[1]);
    Sprite* TileM3 = _meta->getTileAt(tileCoord[2]);
    if ((TileM1) || (TileM2) || (TileM3)) {
        return true;
    }
    return false;
}

void level::Collecting(std::vector<Point> tileCoord, Player1* player) {
    Sprite* TileC1 = _collect->getTileAt(tileCoord[0]);
    Sprite* TileC2 = _collect->getTileAt(tileCoord[1]);
    Sprite* TileC3 = _collect->getTileAt(tileCoord[2]);

    if(TileC1) {
        collecting_items(tileCoord[0], player);
    } else
    if(TileC2) {
        collecting_items(tileCoord[1], player);
    } else
    if(TileC3){
        collecting_items(tileCoord[2], player);
    }
}

void level::isAchievening(std::vector<Point> tileCoord, Player1 *player) {
    Sprite* TileG1 = _goals->getTileAt(tileCoord[0]);
    Sprite* TileG2 = _goals->getTileAt(tileCoord[1]);
    Sprite* TileG3 = _goals->getTileAt(tileCoord[2]);

    if(TileG1) {
        achieving_goals(tileCoord[0], player);
    } else
    if(TileG2) {
        achieving_goals(tileCoord[1], player);
    } else
    if(TileG3){
        achieving_goals(tileCoord[2], player);
    }
}

void level::Collecting_res(std::vector<Point> tileCoord, Player1 *player) {
    Sprite* TileR1 = _resources->getTileAt(tileCoord[0]);
    Sprite* TileR2 = _resources->getTileAt(tileCoord[1]);
    Sprite* TileR3 = _resources->getTileAt(tileCoord[2]);

    if(TileR1) {
        collecting_res(tileCoord[0], player);
    } else
    if(TileR2) {
        collecting_res(tileCoord[1], player);
    } else
    if(TileR3){
        collecting_res(tileCoord[2], player);
    }
}

void level::isWarning(std::vector<Point> tileCoord, Player1 *player) {
    Sprite* TileW1 = _warnings->getTileAt(tileCoord[0]);

    if(TileW1) {
        losing_lives(player); //Возможно надо добавить какую-нибудь анимацию, но это потом
    }
}

void level::exit(Ref* pSender) {
    std::ofstream file_writing("Resources/level_complete.txt");
    file_writing << (1);
    file_writing.close();
    auto menu = menu::createScene();
    Director::getInstance()->replaceScene( TransitionFade::create(3, menu));
}

void level::win() {
    player1->stopAllActions();
    player2->stopAllActions();
    player1->goal = false;
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("complete.mp3", false);
    auto c1 = LevelCompleteScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, c1));
}

void level::goToP2() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto cam = Camera::getDefaultCamera();
    cam->setPosition(975, 320);
    _label->setPosition(Point(origin.x+visibleSize.width + _label->getContentSize().width - 59,
                              origin.y + 30));
    coordinating_exit_button();
}

void level::goToP1() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto cam = Camera::getDefaultCamera();
    cam->setPosition(335, 320);
    _label->setPosition(Point(origin.x+ visibleSize.width/4 + 150,
                              origin.y + 30));
    coordinating_exit_button();
}

void level::lose() {
        auto lvl1 = menu::createScene();
        Director::getInstance()->replaceScene( TransitionFade::create(5, lvl1));
}

void level::create_indicator(Player1* pl) {
    if (pl->get_energy() >= 80) {
        indicators[0]->setPosition(pl->getPosition().x, pl->getPosition().y + 50);
        for (int i = 0; i < 5; i++)
            indicators[i]->setVisible(false);
        indicators[0]->setVisible(true);
    }
    if (pl->get_energy() >= 60 && pl->get_energy() < 80) {
        indicators[1]->setPosition(pl->getPosition().x, pl->getPosition().y + 50);
        for (int i = 0; i < 5; i++)
            indicators[i]->setVisible(false);
        indicators[1]->setVisible(true);
    }
    if (pl->get_energy() >= 40 && pl->get_energy() < 60) {
        indicators[2]->setPosition(pl->getPosition().x, pl->getPosition().y + 50);
        for (int i = 0; i < 5; i++)
            indicators[i]->setVisible(false);
        indicators[2]->setVisible(true);
    }
    if (pl->get_energy() >= 20 && pl->get_energy() < 40) {
        indicators[3]->setPosition(pl->getPosition().x, pl->getPosition().y + 50);
        for (int i = 0; i < 5; i++)
            indicators[i]->setVisible(false);
        indicators[3]->setVisible(true);
    }
    if (pl->get_energy() < 20) {
        indicators[4]->setPosition(pl->getPosition().x, pl->getPosition().y + 50);
        for (int i = 0; i < 5; i++)
            indicators[i]->setVisible(false);
        indicators[4]->setVisible(true);
    }
}

void level::change_imageAD() {
    switch (player_active) {
        case 0: {
            if (abs(player1->getPosition().x - posForChange1.x) > 12) {
                if (image1 == 1) {
                    player1->initWithFile("engineer1_2.png");
                    player1->setScale(0.07);
                    image1 = 2;
                    posForChange1 = player1->getPosition();
                } else {
                    player1->initWithFile("engineer1_1.png");
                    player1->setScale(0.07);
                    image1 = 1;
                    posForChange1 = player1->getPosition();
                }
            }
            break;
        }
        case 1: {
            if (abs(player2->getPosition().x - posForChange2.x) > 12) {
                if (image2 == 1) {
                    player2->initWithFile("engineer2_2.png");
                    player2->setScale(0.07);
                    image2 = 2;
                    posForChange2 = player2->getPosition();
                } else {
                    player2->initWithFile("engineer2_1.png");
                    player2->setScale(0.07);
                    image2 = 1;
                    posForChange2 = player2->getPosition();
                }
            }
            break;
        }
    }
}

void level::change_imageW() {
    SorW = 2;
    switch (player_active) {
        case 0: {
            bool dest = player1->get_dest();
            player1->initWithFile("engineer1_3.png");
            player1->setScale(0.07);
            player1->set_dest(dest);
            player1->setFlippedX(!dest);
        }
        case 1: {
            bool dest = player2->get_dest();
            player2->initWithFile("engineer2_3.png");
            player2->setScale(0.07);
            player2->set_dest(dest);
            player2->setFlippedX(!dest);
        }
    }
}

void level::change_imageS() {
    SorW = 1;
    switch (player_active) {
        case 0: {
            bool dest = player1->get_dest();
            player1->initWithFile("engineer1_1.png");
            player1->setScale(0.07);
            player1->setFlippedX(!dest);
            player1->set_dest(dest);
        }
        case 1: {
            bool dest = player2->get_dest();
            player2->initWithFile("engineer2_1.png");
            player2->setScale(0.07);
            player2->setFlippedX(!dest);
            player2->set_dest(dest);
        }
    }
}

void level::coordinating_exit_button(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    if(player_active == 0){
        closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 + 2,
                                    origin.y + closeItem->getContentSize().height/2));
    }
    else{
        closeItem->setPosition(Vec2(origin.x + 2*visibleSize.width - closeItem->getContentSize().width - 10,
                                    origin.y + closeItem->getContentSize().height/2));
    }
}

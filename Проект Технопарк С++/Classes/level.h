//
// Created by root on 04.05.17.
//

#ifndef MYGAME_LEVEL_H
#define MYGAME_LEVEL_H

#include "cocos2d.h"
#include "Player1.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
class level : public Scene {
public:
    TMXTiledMap* _tileMap;
    TMXLayer* _background;
    TMXLayer* _meta;
    TMXLayer* _collect;
    TMXLayer* _goals;
    TMXLayer* _warnings;
    TMXLayer* _resources;
    TMXLayer* _walls;
    TMXObjectGroup* objectGroup;

    Label* _label;

    Player1* player1;
    Player1 *player2;

    int player_active;
    int number_level;
    int level_amount = 2;


    bool bool1 = true;

    std::array<Point, 3> coor_door{{Point(12, 4), Point(12, 5), Point(12, 6)}};
    std::array<Sprite*, 5> indicators;
    void met(EventKeyboard::KeyCode keyCode, Event* event);
    void metRelease(EventKeyboard::KeyCode keyCode, Event* event);
    bool isKeyPressed(EventKeyboard::KeyCode code);
    virtual void update(float delta) override;

    std::vector<Point> coordination_D(Point pos);
    std::vector<Point> coordination_A(Point pos);
    std::vector<Point> coordination_W(Point pos);
    std::vector<Point> coordination_S(Point pos);

    bool isMeta(std::vector<Point> tileCoord);
    void Collecting(std::vector<Point> tileCoord, Player1* player);
    void isAchievening(std::vector<Point> tileCoord, Player1 *player);
    void Collecting_res(std::vector<Point> tileCoord, Player1 *player);
    void isWarning(std::vector<Point> tileCoord, Player1 *player);
    bool isValue(std::vector<Point> tileCoord, Player1* player);

    void teleportation(Player1* pl);
    void door_accept();
    void goals_compl();

    Point posForChange1;
    Point posForChange2;
    int image2 = 1;
    int image1 = 1;
    void change_imageAD();
    void change_imageW();
    void change_imageS();

    void collecting_items(Point tileCoord, Player1* player);
    void achieving_goals(Point tileCoord, Player1* player);
    void losing_lives(Player1* player);
    void collecting_res(Point tileCoord, Player1* player);

    level* creating_map(char* mapname);
    void creating_person(TMXObjectGroup* og);
    void create_indicator(Player1* pl);
    void menu_staff();

    void goToP2();
    void goToP1();

    void coordinating_exit_button();

    MenuItemImage * closeItem;

    void exit(Ref* pSender);
    void win();
    void lose();
};


#endif //MYGAME_LEVEL_H

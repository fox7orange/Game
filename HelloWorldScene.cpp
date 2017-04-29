#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Player1.h"

USING_NS_CC;


static std::map <cocos2d::EventKeyboard::KeyCode,
        std::chrono::high_resolution_clock::time_point> keys;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    //Добавление карты
    _tileMap = new TMXTiledMap();
    _tileMap->initWithTMXFile("level1.tmx");
    _background = _tileMap->getLayer("background");
    this->addChild(&*_tileMap);

    //Определение недоступных областей карты
    _meta = _tileMap->getLayer("meta");
    _meta->setVisible(false);

    //spawn
    TMXObjectGroup* objectGroup = _tileMap->getObjectGroup("objects");
    auto spawnPoint = objectGroup->getObject("SpawnPoint");
    //get spawn x,y
    int x = spawnPoint.at("x").asInt();
    int y = spawnPoint.at("y").asInt();

    _player = new Sprite();
    _player->initWithFile("engineer.png");
    _player->setPosition(x,y);
    _player->setScale(0.03);

    this->addChild(&*_player);

    auto eventListener = EventListenerKeyboard::create();

    Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
        // If a key already exists, do nothing as it will already have a time stamp
        // Otherwise, set's the timestamp to now
        if(keys.find(keyCode) == keys.end()){
            keys[keyCode] = std::chrono::high_resolution_clock::now();
        }
    };
    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
        // remove the key.  std::map.erase() doesn't care if the key doesnt exist
        keys.erase(keyCode);
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener,this);

    // Let cocos know we have an update function to be called.
    // No worries, ill cover this in more detail later on
    this->scheduleUpdate();


    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    //sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    //this->addChild(sprite, 0);
    return true;
}

bool HelloWorld::isKeyPressed(EventKeyboard::KeyCode code) {
    // Check if the key is currently pressed by seeing it it's in the std::map keys
    // In retrospect, keys is a terrible name for a key/value paried datatype isnt it?
    if(keys.find(code) != keys.end())
        return true;
    return false;
}

void HelloWorld::update(float delta) {
    // Register an update function that checks to see if the CTRL key is pressed
    // and if it is displays how long, otherwise tell the user to press it
    Node::update(delta);
    if (isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
        //Vec2 loc = _player->getPosition();
        auto time1 = std::chrono::system_clock::now();
        while (1) {
            auto time2 = std::chrono::system_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds> (time2 - time1).count() == 25) {
                auto moveBy = MoveBy::create(0.03 , Point(0.5, 0));
                _player->runAction(moveBy);
                break;
            }
        }
    }

    if (isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
        Vec2 loc = _player->getPosition();
        auto time1 = std::chrono::system_clock::now();
        while (1) {
            auto time2 = std::chrono::system_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds> (time2 - time1).count() == 25) {
                _player->setPosition(loc.x - 0.5, loc.y);
                break;
            }
        }
    }
}
//Ничего не понятно
/*void HelloWorld::setPlayerPosition(Point position)
{
    Point tileCoord = HelloWorld.->tileCoordForPosition(position);
    int tileGid = _meta->tileGIDAt(tileCoord);
    if (tileGid) {
        ValueMap properties = _tileMap->getProperties();
        if (properties) {
            std::stringstream *collision = new std::stringstream();
            *collision = *properties->valueForKey("Collidable");
            if (collision && (collision->compare("True") == 0)) {
                return;
            }
        }
    }
    _player->setPosition(position);
}*/


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

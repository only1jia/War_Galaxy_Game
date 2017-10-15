#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "BreakoutScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto start = Label::createWithSystemFont("Start Game", "Arial", 30);

	auto startitem = MenuItemLabel::create(start, CC_CALLBACK_1(HelloWorld::startgame, this));
	startitem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	
	auto menu = Menu::create(startitem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto gameName = Label::createWithSystemFont("Galaxy Wars", "Arial", 50);
	gameName->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 200);
	this->addChild(gameName, 1);

    auto sprite = Sprite::create("bg_menu.jpg");
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    //////////////////////////////
    // 1. super init first
    /*if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);

    return true;*/
}

void HelloWorld::startgame(Ref* re)
{
	auto gamecene = Breakout::createScene();
	Director::getInstance()->replaceScene(gamecene);
}

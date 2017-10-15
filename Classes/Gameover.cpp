#include "Gameover.h"
#include "BreakoutScene.h"

cocos2d::Scene * Gameover::createScene()
{
    auto scene = Scene::create();

    auto layer = Gameover::create();

    scene->addChild(layer);

    return scene;
}

bool Gameover::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

	//添加游戏结束的背景图片
    auto background = Sprite::create("gameover.jpg");
	background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
    background->setPosition(visibleSize / 2);   
    this->addChild(background, 0);

	//添加重新开始的字体以及按钮
	auto label = Label::createWithTTF("Restart", "fonts/Marker Felt.ttf", 60);
	auto start = MenuItemLabel::create(label, CC_CALLBACK_1(Gameover::startMenuCallback, this));
	start->setPosition(visibleSize.width / 3, visibleSize.height / 2 - 100);
	start->setColor(Color3B::RED);

    auto startMenu = Menu::create();
	startMenu->setPosition(visibleSize.width / 3, visibleSize.height / 2 - 100);
	startMenu->addChild(start);
    this->addChild(startMenu, 1);

	return true;
}

void Gameover::startMenuCallback(Ref * pSender)
{
    auto scene = Breakout::createScene();
	Director::getInstance()->replaceScene(TransitionFlipY::create(1, scene));
}

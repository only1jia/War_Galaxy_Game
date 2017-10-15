#include "BreakoutScene.h"
#include "HelloWorldScene.h"
#include <string>
#include "Gamewin.h"
using namespace std;
#include "SimpleAudioEngine.h"
#include "Global.h"

using namespace CocosDenshion;

void Breakout::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

void Breakout::addBackground() {

	auto map3 = Sprite::create("back3_2.jpg");
	auto map4 = Sprite::create("back3_1.jpg");

	map3->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	map4->setPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y - map4->getContentSize().height / 2));

	this->addChild(map3, 0, 27);
	this->addChild(map4, 0, 28);
}

void Breakout::floatBackground(float f) {

	Sprite* temMap3 = (Sprite*)this->getChildByTag(27);
	Sprite* temMap4 = (Sprite*)this->getChildByTag(28);

	temMap3->setPositionY(temMap3->getPositionY() - 1);
	temMap4->setPositionY(temMap4->getPositionY() - 1);



	if (temMap3->getPositionY() + temMap3->getContentSize().height / 2 <= origin.y)
	{
		float offset = temMap3->getPositionY() + temMap3->getContentSize().height / 2 - origin.y;
		temMap3->setPosition(Vec2(visibleSize.width / 2 + origin.x, temMap3->getContentSize().height / 2 + origin.y + visibleSize.height + offset));
	}

	if (temMap4->getPositionY() + temMap4->getContentSize().height / 2 <= origin.x)
	{
		float offset = temMap4->getPositionY() + temMap4->getContentSize().height / 2 - origin.y;
		temMap4->setPosition(Vec2(visibleSize.width / 2 + origin.x, temMap4->getContentSize().height / 2 + origin.y + visibleSize.height + offset));
	}
}

Scene* Breakout::createScene() {
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Point(0, 0));

    auto layer = Breakout::create(scene->getPhysicsWorld());

    scene->addChild(layer);
    return scene;
}

bool Breakout::init(PhysicsWorld* world) {
    if (!Layer::init()) {
        return false;
    }

	this->setPhysicsWorld(world);
    visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	hp = 100;
	bosshp = 500;
	flag = 0;

    preloadMusic();
    playBgm();

    addBackground();
	addlabel();
    addEdge();
    addPlayer();

	addContactListener();
    addKeyboardListener();

    this->schedule(schedule_selector(Breakout::update), 3);
	this->schedule(schedule_selector(Breakout::shoot), 3, kRepeatForever, 1);
	this->scheduleOnce(schedule_selector(Breakout::addboss), 30);
	this->schedule(schedule_selector(Breakout::bossshoot), 1, kRepeatForever, 31);
	this->schedule(schedule_selector(Breakout::bossmove), 3, kRepeatForever, 31);
	this->schedule(schedule_selector(Breakout::floatBackground), 0.01, kRepeatForever, 0);

    return true;
}

Breakout * Breakout::create(PhysicsWorld * world) {
	Breakout* pRet = new(std::nothrow) Breakout();
	if (pRet && pRet->init(world)) {
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}

void Breakout::preloadMusic() {
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/meet_stone.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/fire.wav");
}

void Breakout::playBgm() {
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
}

//void Breakout::addBackground() {
//    auto bgsprite = Sprite::create("bg3_0.png");
//    bgsprite->setPosition(visibleSize / 2);
//    bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width, visibleSize.height / bgsprite->getContentSize().height);
//    this->addChild(bgsprite, 0);
//}

void Breakout::addlabel() {
	hper = Label::createWithSystemFont("HP: 100", "Arial", 30);
	hper->setPosition(Vec2(origin.x + 75, visibleSize.height - 50));
	this->addChild(hper, 1);
}

void Breakout::addEdge() {
    auto edgeSp = Sprite::create();
    auto boundBody = PhysicsBody::createEdgeBox(visibleSize);
	boundBody->getShape(0)->setRestitution(0);
	boundBody->setGroup(-1);
	boundBody->setCategoryBitmask(1);
	boundBody->setCollisionBitmask(1);
	boundBody->setContactTestBitmask(1);
	boundBody->setDynamic(false);
    boundBody->setTag(0);
    edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    edgeSp->setPhysicsBody(boundBody);
    this->addChild(edgeSp);

	auto edgeSp1 = Sprite::create();
	auto boundBody1 = PhysicsBody::createEdgeBox(visibleSize+Size(100, 100));
	boundBody1->getShape(0)->setRestitution(0);
	boundBody1->setGroup(0);
	boundBody1->setCategoryBitmask(3);
	boundBody1->setCollisionBitmask(3);
	boundBody1->setContactTestBitmask(3);
	boundBody1->setDynamic(false);
	boundBody1->setTag(1);
	edgeSp1->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	edgeSp1->setPhysicsBody(boundBody1);
	this->addChild(edgeSp1);
}

void Breakout::addPlayer() {
    player = Sprite::create("player2.png");
	player->setScale(0.35);
	auto size = player->getContentSize();
	auto body = PhysicsBody::createBox(size);
	player->setPhysicsBody(body);
    player->setPosition(visibleSize / 2);
    player->getPhysicsBody()->setTag(3);
	player->getPhysicsBody()->getShape(0)->setRestitution(0);
	player->getPhysicsBody()->setRotationEnable(0);
	player->getPhysicsBody()->setGroup(-3);
	player->getPhysicsBody()->setCategoryBitmask(3);
	player->getPhysicsBody()->setCollisionBitmask(3);
	player->getPhysicsBody()->setContactTestBitmask(3);
	player->getPhysicsBody()->setMass(10000000.0);
    addChild(player);
}

void Breakout::addContactListener() {
	auto touchListener = EventListenerPhysicsContact::create();
	touchListener->onContactBegin = CC_CALLBACK_1(Breakout::onConcactBegan, this);
	_eventDispatcher->addEventListenerWithFixedPriority(touchListener, 1);
}

void Breakout::addKeyboardListener() {
    auto keboardListener = EventListenerKeyboard::create();
    keboardListener->onKeyPressed = CC_CALLBACK_2(Breakout::onKeyPressed, this);
    keboardListener->onKeyReleased = CC_CALLBACK_2(Breakout::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keboardListener, this);
}

void Breakout::attack()
{
	SimpleAudioEngine::getInstance()->playEffect("music/fire.wav");
	auto position = player->getPosition();
	auto bullet = Sprite::create("3-1.png");
	bullet->setScale(0.5);
	bullet->setPosition(position);
	auto size = bullet->getContentSize();
	auto body = PhysicsBody::createBox(size);
	body->getShape(0)->setRestitution(0);
	body->setMass(1.0);
	body->setGroup(-3);
	body->setCategoryBitmask(1);
	body->setCollisionBitmask(1);
	body->setContactTestBitmask(1);
	body->setTag(4);
	body->setVelocity(Vec2(0, 300));
	bullet->setPhysicsBody(body);
	addChild(bullet);
}

void Breakout::launchmissile(int distance)
{
	SimpleAudioEngine::getInstance()->playEffect("music/fire.wav");
	auto position = player->getPosition() + Vec2(distance, 0);
	auto missile = Sprite::create("missile.png");
	missile->setScale(0.7);
	missile->setPosition(position);
	auto size = missile->getContentSize();
	auto body = PhysicsBody::createBox(size);
	body->getShape(0)->setRestitution(0);
	body->setMass(1.0);
	body->setGroup(-3);
	body->setCategoryBitmask(1);
	body->setCollisionBitmask(1);
	body->setContactTestBitmask(1);
	body->setTag(4);
	body->setVelocity(Vec2(0, 500));
	missile->setPhysicsBody(body);
	addChild(missile);
	//auto move = MoveBy::create(0.5, Vec2(distance, 0));
	//missile->runAction(move);
}

bool Breakout::onConcactBegan(PhysicsContact& contact) {
	auto body1 = contact.getShapeA()->getBody();
	auto body2 = contact.getShapeB()->getBody();
	int tag1 = body1->getTag();
	int tag2 = body2->getTag();
	auto sp1 = (Sprite*)body1->getNode();
	auto sp2 = (Sprite*)body2->getNode();
	if (tag1 == 3 && tag2 == 5) collision1(sp1, sp2);
	if (tag1 == 5 && tag2 == 3) collision1(sp2, sp1);
	if (tag1 == 0 && tag2 == 4) collision2(sp1, sp2);
	if (tag1 == 4 && tag2 == 0) collision2(sp2, sp1);
	if (tag1 == 2 && tag2 == 3) collision3(sp1, sp2);
	if (tag1 == 3 && tag2 == 2) collision3(sp2, sp1);
	if (tag1 < 0 && tag2 == 3) collision4(sp1, sp2);
	if (tag1 == 3 && tag2 < 0) collision4(sp2, sp1);
	if (tag1 < 0 && tag2 == 4) collision5(sp1, sp2);
	if (tag1 == 4 && tag2 < 0) collision5(sp2, sp1);
	if (tag1 == 1 && tag2 == 2) collision6(sp1, sp2);
	if (tag1 == 2 && tag2 == 1) collision6(sp2, sp1);
	if (tag1 == 1 && tag2 < 0) collision7(sp1, sp2);
	if (tag1 < 0 && tag2 == 1) collision7(sp2, sp1);
	if (tag1 == 4 && tag2 == 5) collisionboss(sp1, sp2);
	if (tag1 == 5 && tag2 == 4) collisionboss(sp2, sp1);
	return true;
}

void Breakout::update(float f) {
    addEnemy(0);
	addEnemy(1);
	addEnemy(2);
	addEnemy(3);
	addEnemy(4);
}

void Breakout::shoot(float f)
{
	Vector<Sprite*>::iterator it = enemys.begin();
	while (it != enemys.end()) {
		auto position = (*it)->getPosition();
		auto playerposition = player->getPosition();
		Vec2 v = playerposition - position;
		v.normalize();
		Vec2 v1 = v * 100;
		auto bullet = Sprite::create("pd-10.png");
		bullet->setScale(0.3);
		bullet->setPosition(position);
		auto size = bullet->getContentSize();
		auto body = PhysicsBody::createBox(size);
		body->setVelocity(v1);
		body->getShape(0)->setRestitution(0);
		body->setGroup(-2);
		body->setCategoryBitmask(2);
		body->setCollisionBitmask(2);
		body->setContactTestBitmask(2);
		body->setTag(2);
		bullet->setPhysicsBody(body);
		addChild(bullet);
		it++;
	}
}

void Breakout::addEnemy(int type) {
	char temp[10];
	sprintf(temp, "a4-%d.png", type);
	string name(temp);
	auto plane = Sprite::create(name);
	if (type > 0) plane->setScale(0.3);
	float x = random(0.0f, visibleSize.width);
	int vx = random(-100, 100);
	int vy = random(-250, -100);
	plane->setPosition(Vec2(x, visibleSize.height));
	auto planesize = plane->getContentSize();
	auto body = PhysicsBody::createBox(planesize);
	body->getShape(0)->setRestitution(0);
	/*body->setRotationEnable(false);*/
	body->setMass(1000000.0);
	body->setGroup(-1);
	body->setCategoryBitmask(1);
	body->setCollisionBitmask(1);
	body->setContactTestBitmask(1);
	body->setTag(-1);
	body->setVelocity(Vec2(vx, vy));
	plane->setPhysicsBody(body);
	addChild(plane);
	enemys.pushBack(plane);
}

void Breakout::addboss(float f)
{
	boss = Sprite::create("boss2.png");
	boss->setScale(0.4);
	auto size = boss->getContentSize();
	auto body = PhysicsBody::createBox(size);
	body->getShape(0)->setRestitution(0);
	body->setGroup(-1);
	body->setCategoryBitmask(1);
	body->setCollisionBitmask(1);
	body->setContactTestBitmask(1);
	body->setTag(5);
	body->setRotationEnable(0);
	boss->setPhysicsBody(body);
	boss->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
	addChild(boss);
	auto move = MoveBy::create(0.75, Vec2(0, -100));
	boss->runAction(move);
}

void Breakout::bossshoot(float f)
{
	auto position = boss->getPosition();
	auto bullet = Sprite::create("2-2.png");
	bullet->setScale(0.4);
	auto size = bullet->getContentSize();
	auto body = PhysicsBody::createBox(size);
	body->getShape(0)->setRestitution(0);
	body->setGroup(-2);
	body->setCategoryBitmask(2);
	body->setCollisionBitmask(2);
	body->setContactTestBitmask(2);
	body->setTag(2);
	body->setVelocity(Vec2(0, -100));
	bullet->setPhysicsBody(body);
	bullet->setPosition(position);
	addChild(bullet);
}

void Breakout::bossmove(float f)
{
	auto size = boss->getContentSize();
	if (flag == 0) {
		auto moveleft = MoveTo::create(2.75, Vec2(origin.x, visibleSize.height-100));
		boss->runAction(moveleft);
		flag = 1;
	}
	else {
		auto moveright = MoveTo::create(2.75, Vec2(visibleSize.width, visibleSize.height-100));
		boss->runAction(moveright);
		flag = 0;
	}
}

void Breakout::collision1(Sprite * gamer, Sprite * enemyboss)
{
	hp -= 100;
	updatehp();
}

void Breakout::collision2(Sprite * inwall, Sprite * mybullet)
{
	if (mybullet != nullptr)
	mybullet->removeFromParentAndCleanup(1);
}

void Breakout::collision3(Sprite * enemybullet, Sprite * gamer)
{
	if (enemybullet == nullptr) return;
	enemybullet->removeFromParentAndCleanup(1);
	hp -= 20;
	updatehp();
}

void Breakout::collision4(Sprite * enemy, Sprite * gamer)
{
	enemys.eraseObject(enemy);
	if (enemy == nullptr) return;
	SimpleAudioEngine::getInstance()->playEffect("music/meet_stone.wav");
	auto death = ParticleSystemQuad::create("explode.plist");
	death->setPosition(enemy->getPosition());
	death->setAutoRemoveOnFinish(1);
	death->setDuration(0.5);
	this->addChild(death, 1);
	enemy->removeFromParentAndCleanup(1);
	hp -= 25;
	updatehp();
}

void Breakout::collision5(Sprite * enemy, Sprite * mybullet)
{
	if (enemy == nullptr || mybullet == nullptr) return;
	mybullet->removeFromParentAndCleanup(1);
	auto body = enemy->getPhysicsBody();
	int newtag = body->getTag() - 20;
	body->setTag(newtag);
	if (newtag < -100) {
		enemys.eraseObject(enemy);
		SimpleAudioEngine::getInstance()->playEffect("music/meet_stone.wav");
		auto death = ParticleSystemQuad::create("explode.plist");
		death->setPosition(enemy->getPosition());
		death->setAutoRemoveOnFinish(1);
		death->setDuration(0.5);
		this->addChild(death, 1);
		enemy->removeFromParentAndCleanup(1);
	}
}

void Breakout::collision6(Sprite * outwall, Sprite * enemybullet)
{
	if (enemybullet != nullptr)
	enemybullet->removeFromParentAndCleanup(1);
}

void Breakout::collision7(Sprite * outwall, Sprite * enemy)
{
	enemys.eraseObject(enemy);
	if (enemy != nullptr)
	enemy->removeFromParentAndCleanup(1);
}

void Breakout::collisionboss(Sprite * mybullet, Sprite * boss)
{
	if (mybullet != nullptr)
	mybullet->removeFromParentAndCleanup(1);
	bosshp -= 10;
	if (bosshp < 0) {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		_eventDispatcher->removeAllEventListeners();
		//MessageBoxA(WindowFromDC(wglGetCurrentDC()), "You win!", "Congratulation", MB_OK);
		Global::score = hp;
		auto scene = Gamewin::createScene();
		Director::getInstance()->replaceScene(TransitionJumpZoom::create(1, scene));
		
	}
}

void Breakout::updatehp()
{
	char temp[5];
	sprintf(temp, "%d", hp);
	string temp1(temp);
	string temp2 = "HP: " + temp1;
	hper->setString(temp2);
	if (hp < 1) {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		_eventDispatcher->removeAllEventListeners();
		MessageBoxA(WindowFromDC(wglGetCurrentDC()), "You died!", "Game Over", MB_OK);
		auto scene = HelloWorld::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void Breakout::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
    switch (code)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        player->getPhysicsBody()->setVelocity(Point(-250, player->getPhysicsBody()->getVelocity().y));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        player->getPhysicsBody()->setVelocity(Point(250, player->getPhysicsBody()->getVelocity().y));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        player->getPhysicsBody()->setVelocity(Point(player->getPhysicsBody()->getVelocity().x, 250));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        player->getPhysicsBody()->setVelocity(Point(player->getPhysicsBody()->getVelocity().x, -250));
        break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		attack();
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_L:
		launchmissile(-25);
		launchmissile(25);
		break;
    default:
        break;
    }
}

void Breakout::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
    switch (code)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(-250, 0));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(250, 0));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(0, 250));
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_S:
        player->getPhysicsBody()->setVelocity(player->getPhysicsBody()->getVelocity() - Point(0, -250));
        break;
    default:
        break;
    }
}

#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class Breakout :public Layer {
public:
	void setPhysicsWorld(PhysicsWorld * world);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init(PhysicsWorld* world);

    // implement the "static create()" method manually
	static Breakout* create(PhysicsWorld* world);

private:
    Sprite* player;
	Sprite* boss;
	PhysicsWorld* m_world;
	Size visibleSize;
	Vec2 origin;
	Vector<Sprite*> enemys;
	Label* hper;

	int hp;
	bool flag;
	int bosshp;

    void preloadMusic();
    void playBgm();

    void addBackground();
    void addEdge();
	void addPlayer();

	void addContactListener();
	void addKeyboardListener();

	void attack();
	void launchmissile(int distance);

	void update(float f);
	void shoot(float f);

	bool onConcactBegan(PhysicsContact& contact);
    void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode code, Event* event);

	void addEnemy(int type);
	void addboss(float f);
	void bossshoot(float f);
	void bossmove(float f);

	void collision1(Sprite* gamer, Sprite* enemyboss);
	void collision2(Sprite* inwall, Sprite* mybullet);
	void collision3(Sprite* enemybullet, Sprite* gamer);
	void collision4(Sprite* enemy, Sprite* gamer);
	void collision5(Sprite* enemy, Sprite* mybullet);
	void collision6(Sprite* outwall, Sprite* enemybullet);
	void collision7(Sprite* outwall, Sprite* enemy);
	void collisionboss(Sprite* mybullet, Sprite* boss);

	void updatehp();
	void addlabel();
	void floatBackground(float f);
};
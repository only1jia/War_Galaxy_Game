#ifndef __GAMEOVER_H__
#define __GAMEOVER_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class Gameover : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(Gameover);

	void startMenuCallback(Ref* pSender);

};

#endif // __GAMEOVER_H__

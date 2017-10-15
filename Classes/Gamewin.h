#ifndef __GAMEWIN_H__
#define __GAMEWIN_H__

#include "cocos2d.h"
#include <vector>
#include "ui/CocosGUI.h"
#include <string>
#include "Global.h"
using namespace cocos2d::ui;
#include "network/HttpClient.h"
using namespace cocos2d::network;
USING_NS_CC;

class Gamewin : public cocos2d::Layer
{
public:

	float visibleHeight;
	float visibleWidth;
	TextField * rank_field;
	Button *rank_button;
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	void Submit();
	void Rank();
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
	// implement the "static create()" method manually
	CREATE_FUNC(Gamewin);


};

#endif // __GAMEWIN_H__

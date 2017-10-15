#include "Gamewin.h"
#include "BreakoutScene.h"
#include "ui/CocosGUI.h"
#include <string>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#include <iostream>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
using namespace std;
#define database UserDefault::getInstance()

USING_NS_CC;

cocos2d::Scene * Gamewin::createScene()
{
	auto scene = Scene::create();

	auto layer = Gamewin::create();

	scene->addChild(layer);

	return scene;
}

bool Gamewin::init()
{
    if (!Layer::init()) {
        return false;
    }
	Submit();
	Size size = Director::getInstance()->getVisibleSize();
	visibleHeight = size.height;
	visibleWidth = size.width;

	TMXTiledMap* tmx = TMXTiledMap::create("map.tmx");
	tmx->setPosition(size / 2);
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setScale(Director::getInstance()->getContentScaleFactor());
	this->addChild(tmx, 0);
	////添加游戏胜利背景图片
 //   auto background = Sprite::create("gamewin.jpg");
	//background->setScale(size.width / background->getContentSize().width, size.height / background->getContentSize().height);
	//background->setPosition(size / 2);	
	//this->addChild(background, 0);
	auto label = Label::create("name score", "Arial", 30);
	label->setPosition(Size(visibleWidth / 2 + 120, visibleHeight / 2 + label->getContentSize().height + 150));
	addChild(label);

	rank_field = TextField::create("", "Arial", 30);
	rank_field->setPosition(Size(visibleWidth / 2 + 120, visibleHeight / 2 -  label->getContentSize().height - rank_field->getContentSize().height));
	this->addChild(rank_field, 2);

	rank_button = Button::create();
	rank_button->setTitleText("Rank");
	rank_button->setTitleFontSize(30);
	rank_button->setPosition(Size(visibleWidth  - rank_button->getContentSize().width, visibleHeight - 80));
	this->addChild(rank_button, 2);

	rank_button->addClickEventListener(CC_CALLBACK_0(Gamewin::Rank, this));

	auto l = Label::createWithTTF("Restart", "fonts/Marker Felt.ttf", 60);
	auto start = MenuItemLabel::create(l, [&](Ref*) {
		auto scene = Breakout::createScene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0, scene));
	});
	start->setPosition(l->getContentSize().width, l->getContentSize().height + 20);
	start->setColor(Color3B::BLUE);

	auto startMenu = Menu::create(start, nullptr);
	startMenu->setPosition(Vec2::ZERO);
	this->addChild(startMenu, 1);

	return true;
}


void Gamewin::Submit() {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/submit");
	request->setRequestType(HttpRequest::Type::POST);
	//request->setResponseCallback(CC_CALLBACK_2(GameScene::onHttpRequestCompleted, this));
	stringstream str;
	str << Global::score;
	string _postData = "score=" + str.str();;

	const char* postData = _postData.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("Post Submit");

	vector<string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}


void Gamewin::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
	if (!response) {
		return;
	}
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	std::vector<char> *buffer = response->getResponseData();
	rapidjson::Document d;
	d.Parse<0>(Global::toString(buffer).c_str());
	if (d.HasParseError())
	{
		log("GetPareseError %s\n", d.GetParseError());
		return;
	}
	if (d.IsObject() && d.HasMember("result")) {
		if (d["result"].GetBool()) {
			string top_rank = d["info"].GetString();
			top_rank.erase(top_rank.begin());
			for (unsigned int i = 0; i < top_rank.length(); i++) {
				if (top_rank[i] == '|')
					top_rank[i] = '\n';
			}
			rank_field->setString(top_rank);
		}
	}

	printf("Http Test, dum data: ");
	for (unsigned int i = 0; i < buffer->size(); i++) {
		printf("%c", (*buffer)[i]);
	}
	printf("\n");
}

void Gamewin::Rank() {
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/rank?top=10");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(Gamewin::onHttpRequestCompleted, this));
	request->setTag("Get Rank");

	vector<string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + Global::gameSessionId);
	request->setHeaders(headers);
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}


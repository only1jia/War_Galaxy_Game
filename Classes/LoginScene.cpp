#include "LoginScene.h"
#include "HelloWorldScene.h"
#include "Global.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#include <network\HttpClient.h>
#define database UserDefault::getInstance()
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
using namespace cocos2d::network;
USING_NS_CC;

using namespace cocostudio::timeline;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


// on "init" you need to initialize your instance
bool LoginScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }


    Size size = Director::getInstance()->getVisibleSize();
    visibleHeight = size.height;
    visibleWidth = size.width;

	auto background = Sprite::create("background.jpg");
	background->setPosition(visibleWidth / 2, visibleHeight / 2);
	background->setScale(visibleWidth / background->getContentSize().width, visibleHeight / background->getContentSize().height);
	this->addChild(background, 0);

	auto playerName = Label::createWithSystemFont("Player Name: ", "Arial", 25);
	playerName->setPosition(Size(visibleWidth / 2 - 100, visibleHeight / 4 * 3));
	this->addChild(playerName, 1);

    textField = TextField::create("Input here", "Arial", 25);
    textField->setPosition(Size(visibleWidth / 2 + 50, visibleHeight / 4 * 3));
    this->addChild(textField, 2);

	auto password = Label::createWithSystemFont("Password: ", "Arial", 25);
	password->setPosition(Size(visibleWidth / 2 - 100, visibleHeight / 8 * 5));
	this->addChild(password, 1);

	passwordField = TextField::create("Input here", "Arial", 25);
	passwordField->setPasswordEnabled(true);
	passwordField->setPasswordStyleText("*");
	passwordField->setPosition(Size(visibleWidth / 2 + 50, visibleHeight / 8 * 5));
	this->addChild(passwordField, 2);

    auto button = Button::create();
    button->setTitleText("Login");
    button->setTitleFontSize(28);
	//��ӵ�¼��ť�Ļص���������������Ϸ�����л�
	button->addClickEventListener(CC_CALLBACK_1(LoginScene::LoginCallback, this));
	button->setPosition(Size(visibleWidth / 2, visibleHeight / 2));
    this->addChild(button, 2);

	////����Զ���¼��ť
	//auto autoButton = Button::create();
	//autoButton->setTitleText("AutoLogin");
	//autoButton->setTitleFontSize(25);
	//��ӵ�¼��ť�Ļص���������������Ϸ�����л�
	/*autoButton->addClickEventListener(CC_CALLBACK_1(LoginScene::AutoLogin, this));
	autoButton->setPosition(Size(visibleWidth / 2 + 100, visibleHeight / 2));
	this->addChild(autoButton, 2);*/
    return true;
}

void LoginScene::LoginCallback(Ref * pSender) {
	string password = database->getStringForKey("Password");
	string name = database->getStringForKey("Name");
	if (textField->getString() == "" || passwordField->getString() == "") return;
	if (name == "" || name != textField->getString()
		|| (name == textField->getString() && password == passwordField->getString())) {
		//ʹ��HttpRequest�޲����Ĺ��캯������ʵ��
		HttpRequest* request = new HttpRequest();
		//�������ӷ��������ͺʹ����ӵĵ�ַ
		request->setRequestType(HttpRequest::Type::POST);
		request->setUrl("http://localhost:8080/login");
		//���ûص�
		request->setResponseCallback(CC_CALLBACK_2(LoginScene::onHttpComplete, this));
		string temp = "username=" + textField->getString();
		const char* postData = temp.c_str();
		request->setRequestData(postData, strlen(postData));
		request->setTag("POST test");

		//�������HttpClient�������
		cocos2d::network::HttpClient::getInstance()->send(request);
		//�ͷ�����
		request->release();

		database->setStringForKey("Name", textField->getString());
		database->setStringForKey("Password", passwordField->getString());
		//���г����л�
		auto scene = HelloWorld::createScene();
		Director::getInstance()->replaceScene(TransitionFlipY::create(1, scene));
	}
}

void LoginScene::onHttpComplete(HttpClient * sender, HttpResponse * response) {
	if (!response) return;
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
	std::vector<char>*buffer = response->getResponseData();
	log("Http Test, dump data: ");
	string temp = Global::toString(buffer);
	log(temp.c_str());

	log("Header:");
	std::vector<char>*headers = response->getResponseHeader();
	temp = Global::toString(headers);
	log(temp.c_str());
	//����buffer�е�GAMESESSIONID
	string ID = "";
	for (unsigned int i = 0; i < temp.size(); i++) {
		if (temp[i - 1] == '=') {
			for (unsigned int j = i; temp[j] != '\r'; j++)
				ID.push_back(temp[j]);
			break;
		}
	}
	//��ȡ��������ID
	Global::gameSessionId = ID;
	//���汾�ε�¼��GAMESESSIONID
	database->setStringForKey("ID", ID);
}

//void LoginScene::AutoLogin(Ref * pSender) {
//	string id = database->getStringForKey("ID");
//	string name = database->getStringForKey("Name");
//	if (id != "") Global::gameSessionId = id;
//	else return;
//	//ʹ��HttpRequest�޲����Ĺ��캯������ʵ��
//	HttpRequest* request = new HttpRequest();
//	//�������ӷ��������ͺʹ����ӵĵ�ַ
//	request->setRequestType(HttpRequest::Type::POST);
//	request->setUrl("http://localhost:8080/login");
//	
//	string temp = "username=" + name;
//	const char* postData = temp.c_str();
//	request->setRequestData(postData, strlen(postData));
//	request->setTag("POST test");
//
//	vector<string> headers;
//	request->setHeaders(headers);
//
//	//�������HttpClient�������
//	cocos2d::network::HttpClient::getInstance()->send(request);
//	//�ͷ�����
//	request->release();
//
//	auto scene = HelloWorld::createScene();
//	Director::getInstance()->replaceScene(TransitionFlipY::create(1, scene));
//}



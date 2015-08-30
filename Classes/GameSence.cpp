#include "GameSence.h"
#include "AirHockyScene.h"
#include "HelloWorldScene.h"
#include "resource.h"

using namespace cocos2d;

Scene* GameSence::createScene()
{
	auto sence=Scene::create();
	auto layer=GameSence::create();
	sence->addChild(layer);
	return sence;
}
bool GameSence::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (!Layer::init())
		return false;
	//添加第一个精灵
	auto spirit01=Sprite::create("Blue_Front1.png");
	spirit01->setAnchorPoint(Vec2(0.5,0.5));
	spirit01->setScale(0.5);
	spirit01->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	spirit01->setTag(110);
	this->addChild(spirit01,0);
	//创建事件侦听对象
	auto listener=EventListenerTouchOneByOne::create();
	//定义侦听对象的回调方法
	listener->onTouchBegan=CC_CALLBACK_2(GameSence::onTouchBegan,this);
	listener->onTouchMoved=CC_CALLBACK_2(GameSence::onTouchMoved,this);
	listener->onTouchEnded=CC_CALLBACK_2(GameSence::onTouchEnded,this);
	//在事件分发器中进行注册
	_eventDispatcher->EventDispatcher::addEventListenerWithSceneGraphPriority(listener, this);
	//添加一个返回主菜单按钮
	auto homeItem = MenuItemImage::create(
		"CloseSelected.png",
		"CloseNormal.png",
		CC_CALLBACK_1(GameSence::menuCallback, this));

	homeItem->setPosition(Vec2(origin.x + visibleSize.width - homeItem->getContentSize().width/2 ,
		origin.y + homeItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(homeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//添加计划任务
	this->scheduleUpdate();
	return true;
}
void GameSence::menuCallback(cocos2d::Ref* pSender)
{
	auto homesence=HelloWorld::createScene();
	auto scene02=TransitionZoomFlipY::create(0.4,homesence);
	Director::getInstance()->replaceScene(scene02);
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto spirit=getChildByTag(110);
	spirit->setPosition(Point(touch->getLocation().x,touch->getLocation().y));
	SimpleAudioEngine::getInstance()->playBackgroundMusic( WAV_HIT );
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
	return true;
}
void GameSence::onTouchMoved(Touch *touch, Event *unused_event)
{
	auto spirit=getChildByTag(110);
	spirit->setPosition(Point(touch->getLocation().x,touch->getLocation().y));

}
void GameSence::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto spirit=getChildByTag(110);
	spirit->setPosition(Point(touch->getLocation().x,touch->getLocation().y));
	SimpleAudioEngine::getInstance()->playBackgroundMusic( WAV_SOCRE );
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
}
void GameSence::update(float t)
{
	auto spirit=getChildByTag(110);
	spirit->setPositionX(spirit->getPositionX()+1);
	if(spirit->getPositionX()>=480)
		spirit->setPositionX(0);
}
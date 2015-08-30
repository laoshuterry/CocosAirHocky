#include "HelloWorldScene.h"
#include "AirHockyScene.h"
#include "resource.h"

Scene* AirHockyScene::createScene()
{
	auto sence=Scene::create();
	auto layer=AirHockyScene::create();
	sence->addChild(layer);
	return sence;
}
bool AirHockyScene::init()
{
	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();
	if (!Layer::init())
		return false;

	_play01Score=0;
	_play02Score=0;

	//添加背景桌面
	auto court=Sprite::create("AirHocky/sd/court.png");
	court->setPosition(Vec2(origin.x + visibleSize.width/2 ,origin.y + visibleSize.height/2));
	this->addChild(court,-1);
	//添加精灵-球
	_ball=AirHockySprite::create("AirHocky/sd/puck.png");
	_ball->setPosition(Vec2(origin.x+visibleSize.width*0.5f,origin.y+visibleSize.height*0.5f));
	this->addChild(_ball,1);
	////添加精灵-玩家1
	_play01=AirHockySprite::create("AirHocky/sd/mallet.png");
	players.pushBack(_play01);
	_play01->setPosition(Vec2(origin.x+visibleSize.width*0.5f,origin.y+_play01->radius()*2));
	this->addChild(_play01,1);
	//添加精灵-玩家2
	_play02=AirHockySprite::create("AirHocky/sd/mallet.png");
	players.pushBack(_play02);
	_play02->setPosition(Vec2(visibleSize.width*0.5f,visibleSize.height-_play01->radius()*2));
	this->addChild(_play02,1);

	_play01ScoreLabel=Label::createWithTTF("0", "fonts/arial.ttf", 60);
	_play01ScoreLabel->setPosition(Vec2(visibleSize.width-60,visibleSize.height*0.5f-80));
	_play01ScoreLabel->setRotation(90);
	this->addChild(_play01ScoreLabel,1);

	_play02ScoreLabel=Label::createWithTTF("0", "fonts/arial.ttf", 60);
	_play02ScoreLabel->setPosition(Vec2(visibleSize.width-60,visibleSize.height*0.5f+80));
	_play02ScoreLabel->setRotation(90);
	this->addChild(_play02ScoreLabel,1);

	//添加一个返回主菜单按钮
	auto homeItem = MenuItemImage::create(
		"CloseSelected.png",
		"CloseNormal.png",
		CC_CALLBACK_1(AirHockyScene::menuCallback, this));
	homeItem->setPosition(Vec2(origin.x + visibleSize.width - homeItem->getContentSize().width/2 ,
		origin.y + homeItem->getContentSize().height/2));
	// create menu, it's an autorelease object
	auto menu = Menu::create(homeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//添加计划任务
	auto listener=EventListenerTouchAllAtOnce::create();
	//定义侦听对象的回调方法
	listener->onTouchesBegan=CC_CALLBACK_2(AirHockyScene::onTouchesBegan,this);
	listener->onTouchesMoved=CC_CALLBACK_2(AirHockyScene::onTouchesMoved,this);
	listener->onTouchesEnded=CC_CALLBACK_2(AirHockyScene::onTouchesEnded,this);
	//在事件分发器中进行注册
	_eventDispatcher->EventDispatcher::addEventListenerWithSceneGraphPriority(listener, this);
	this->scheduleUpdate();
	return true;
}
void AirHockyScene::menuCallback(cocos2d::Ref* pSender)
{
	auto homesence=HelloWorld::createScene();
	Director::getInstance()->replaceScene(homesence);
}
void AirHockyScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	for(auto touch: touches)
		if(touch!=nullptr)
		{
			auto tap=touch->getLocation();
			for(auto player:players)
				if(player->boundingBox().containsPoint(tap))
				{
					player->setOpacity(240);
					player->setTouch(touch);
				}
		}
}
void AirHockyScene::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
		for(auto touch: touches)
		if(touch!=nullptr)
		{
			auto tap=touch->getLocation();
			for(auto player:players)
				if(player->getTouch()!=nullptr&&player->getTouch()==touch)
				{
					Point nextPosition=tap;
					if(nextPosition.x<player->radius())
						nextPosition.x=player->radius();
					if(nextPosition.x>visibleSize.width-player->radius())
						nextPosition.x=visibleSize.width-player->radius();
					if(nextPosition.y<player->radius())
						nextPosition.y=player->radius();
					if(nextPosition.y>visibleSize.height-player->radius())
						nextPosition.y=visibleSize.height-player->radius();

					if(player->getPositionY()<visibleSize.height*0.5f)
					{
						if(nextPosition.y>visibleSize.height*0.5f-player->radius())
							nextPosition.y=visibleSize.height*0.5f-player->radius();
					}
					else
					{
						if(nextPosition.y<visibleSize.height*0.5f+player->radius())
							nextPosition.y=visibleSize.height*0.5f+player->radius();
					}
					player->setNextPosition(nextPosition);
					player->setVector(Vec2(tap.x-player->getPositionX(),tap.y-player->getPositionY()));
				}
		}
}
void AirHockyScene::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	for(auto touch: touches)
		if(touch!=nullptr)
		{
			auto tap=touch->getLocation();
			for(auto player:players)
				if(player->getTouch()!=nullptr&&player->getTouch()==touch)
				{
					player->setOpacity(255);
					player->setTouch(nullptr);
					player->setVector(Vec2(0,0));
				}
		}
}
void AirHockyScene::update(float t)
{
	//对球的运动加入摩擦系数，按程序的配置，目前程序是每秒60帧，程序渲染每帧时，都会执行UPDATE函数，也就是每秒执行60次update函数。
	//这里对球的向量加入了0.98的摩擦系数，也就是每次刷新画面时，球会循环递减速度，从而实现了摩擦的效果
	auto ballNextPosition = _ball->getNextPosition();
	auto ballVector = _ball->getVector();
	//这里使用的是标量与向量的相乘，A=(x,y),B=kA=k(x,y)=(kx,ky)
	ballVector *= 0.98f;
	ballNextPosition.x += ballVector.x;
	ballNextPosition.y += ballVector.y;

	//碰撞判断
	float standardDistance=pow(_play01->radius()+_ball->radius(),2);
	for(auto player: players)
	{
		auto playerNextPosition=player->getNextPosition();
		auto playerVector=player->getVector();
		//计算两种距离方式，一种是球当前坐标与手柄距离，一个是球的下一坐标与手柄的距离
		float distance1=pow(_ball->getPositionX()-player->getNextPosition().x,2)+pow(_ball->getPositionY()-player->getNextPosition().y,2);
		float distance2=pow(_ball->getNextPosition().x-player->getPositionX(),2)+pow(_ball->getNextPosition().y-player->getPositionY(),2);
		if (distance1<=standardDistance||distance2<=standardDistance)
		{
			float mag_ball = pow(ballVector.x, 2) + pow(ballVector.y,2);
			float mag_player = pow(playerVector.x, 2) + pow(playerVector.y, 2);

			float force = sqrt(mag_ball+mag_player);
			float angle = atan2(_ball->getNextPosition().y-player->getPositionY(), _ball->getNextPosition().x-player->getPositionX());

			ballVector.x = force * cos(angle);
			ballVector.y = (force * sin(angle));

			ballNextPosition.x = playerNextPosition.x + (player->radius() + _ball->radius() + force) * cos(angle);
			ballNextPosition.y = playerNextPosition.y + (player->radius() + _ball->radius() + force) * sin(angle);

			SimpleAudioEngine::getInstance()->playEffect(WAV_HIT);
			_ball->setOpacity(200);
		}
	}

	//X轴方向边缘碰撞检测
	if (ballNextPosition.x<_ball->radius())
	{
		ballNextPosition.x=_ball->radius();
		ballVector.x*=-0.8f;
		SimpleAudioEngine::getInstance()->playEffect(WAV_HIT);
	}
	if(ballNextPosition.x>visibleSize.width-_ball->radius())
	{
		ballNextPosition.x=visibleSize.width-_ball->radius();
		ballVector.x*=-0.8f;
		SimpleAudioEngine::getInstance()->playEffect(WAV_HIT);
	}

	if(ballNextPosition.y<_ball->radius())
	{
		if(_ball->getPosition().x<visibleSize.width*0.5f - WIDTH_GATE*0.5f||
			_ball->getPosition().x>visibleSize.width*0.5f + WIDTH_GATE*0.5f)
		{
			ballNextPosition.y=_ball->radius();
			ballVector.y*=-0.8f;
			SimpleAudioEngine::getInstance()->playEffect(WAV_HIT);
		}
	}
	if(ballNextPosition.y>visibleSize.height-_ball->radius())
	{
		if(_ball->getPosition().x <visibleSize.width*0.5f - WIDTH_GATE*0.5f||
			_ball->getPosition().x>visibleSize.width*0.5f + WIDTH_GATE*0.5f)
		{
			ballNextPosition.y=visibleSize.height-_ball->radius();
			ballVector.y*=-0.8f;
			SimpleAudioEngine::getInstance()->playEffect(WAV_HIT);
		}
	}

	_ball->setVector(ballVector);
	_ball->setNextPosition(ballNextPosition);

	if(ballNextPosition.y<-_ball->radius()*2)
		this->setScore(2);
	if(ballNextPosition.y>visibleSize.height+_ball->radius()*2)
		this->setScore(1);

	//在程序的触摸响应中，始终没有对精灵的配置进行设置，而只是设置触摸点的下一位置和向量
	//其目的是在这里统一对3个精灵的坐标做设置，减少了画面抖动，规划更合理
	_play01->setPosition(_play01->getNextPosition());
	_play02->setPosition(_play02->getNextPosition());
	_ball->setPosition(_ball->getNextPosition());
}
void AirHockyScene::setScore(int play)
{
	SimpleAudioEngine::getInstance()->playEffect(WAV_SOCRE);
	_ball->setVector(Vec2(0,0));

	if(play==1)
	{
		_play01Score++;
		_play01ScoreLabel->setString(std::to_string(_play01Score));
		_ball->setPosition(Vec2(visibleSize.width*0.5f,visibleSize.height*0.5f-2*_ball->radius()));
	}
	if(play==2)
	{
		_play02Score++;
		_play02ScoreLabel->setString(std::to_string(_play02Score));
		_ball->setPosition(Vec2(visibleSize.width*0.5f,visibleSize.height*0.5f+2*_ball->radius()));
	}

	_play01->setPosition(Vec2(origin.x+visibleSize.width*0.5f,origin.y+_play01->radius()*2));
	_play02->setPosition(Vec2(visibleSize.width*0.5f,visibleSize.height-_play01->radius()*2));

	_play01->setTouch(nullptr);
	_play02->setTouch(nullptr);
}	
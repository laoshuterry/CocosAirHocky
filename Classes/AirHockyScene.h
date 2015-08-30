#ifndef __AIRHOCKYSCENE_H__
#define __AIRHOCKYSCENE_H__
#include "cocos2d.h"
#include "AirHockySprite.h"
#define WIDTH_GATE 400
using namespace cocos2d;
class AirHockyScene : public Layer
{
	AirHockySprite* _play01;
	AirHockySprite* _play02;
	AirHockySprite* _ball;

	Vector<AirHockySprite*> players;

	Label* _play01ScoreLabel;
	Label* _play02ScoreLabel;

	int _play01Score;
	int _play02Score;
	void setScore(int play);

	Size visibleSize;
	Vec2 origin;

public:
	static cocos2d::Scene* createScene();
	virtual bool	init();
	void			menuCallback(cocos2d::Ref* pSender);
    void			onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void			onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void			onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void			update(float t);
	CREATE_FUNC(AirHockyScene);
};
#endif // __GAMESENCE_H__
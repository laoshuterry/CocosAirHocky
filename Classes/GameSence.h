#ifndef __GAMESENCE_H__
#define __GAMESENCE_H__

#include "cocos2d.h"
using namespace cocos2d;
class GameSence : public Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool	init();
	void			menuCallback(cocos2d::Ref* pSender);
	bool			onTouchBegan(Touch *touch, Event *unused_event);
	void			onTouchMoved(Touch *touch, Event *unused_event);
	void			onTouchEnded(Touch *touch, Event *unused_event);
	void			update(float t);
	CREATE_FUNC(GameSence);
};
#endif // __GAMESENCE_H__
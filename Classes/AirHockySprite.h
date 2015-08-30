#ifndef __AIRHOCKYSPRITE_H__
#define __AIRHOCKYSPRITE_H__
#include "cocos2d.h"
using namespace cocos2d;
class AirHockySprite : public Sprite
{
public:
	AirHockySprite();
	virtual ~AirHockySprite();
	CC_SYNTHESIZE(Vec2,_vector,Vector);
	CC_SYNTHESIZE(Vec2,_nextPosition,NextPosition);
	CC_SYNTHESIZE(Touch*,_touch,Touch);

	static AirHockySprite* create(const std::string& filename);
	virtual void setPosition(const Vec2& pos) override;
	float radius();
};
#endif
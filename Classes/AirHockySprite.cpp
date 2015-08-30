#include "AirHockySprite.h"
AirHockySprite::AirHockySprite()
{
	_vector = Vec2(0,0);
}
AirHockySprite::~AirHockySprite()
{
}
AirHockySprite* AirHockySprite::create(const std::string& filename)
{
    auto sprite = new (std::nothrow) AirHockySprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
void AirHockySprite::setPosition(const Vec2& pos)
{
	Sprite::setPosition(pos);
	if (!_nextPosition.equals(pos))
	{
		_nextPosition = pos;
	}
}
float AirHockySprite::radius()
{
	return getTexture()->getContentSize().width*0.5f;
}

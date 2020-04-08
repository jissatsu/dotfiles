#ifndef __CURSOR_HH
#define __CURSOR_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"

USING_NS_CC;

class Cursor
{
    public:
        Cursor( Node *node, const Vec2& coords, float scaleX, float scaleY, const std::string& img );
        ~Cursor();
        void pulse( float tm );
        Layer * GetLayer( void ) const;
    
    private:
        Layer *_layer;
        Sprite *_img;
};

#endif

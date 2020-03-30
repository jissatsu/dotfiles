#ifndef __OPTION_LIST_VIEW_HH
#define __OPTION_LIST_VIEW_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"

USING_NS_CC;

class OptionListView
{
    public:
        OptionListView(Node *parent, const std::string& bg, const Vec2& coords);
        ~OptionListView();
        Layer * GetLayer( void ) const;
    
    public:
        Layer *_layer;
        Sprite *_background;
};

#endif
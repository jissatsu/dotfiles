#ifndef __WINDOW_BASE_HH
#define __WINDOW_BASE_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"

USING_NS_CC;

namespace d_lan {

    class WindowBase : public Layer
    {
        public:
            virtual bool init_base( Node *parent, const Vec2& coords, short z_index );
            void set_background( std::string bg, float opacity );
            virtual void hide( const std::function<void()>& callback, float hide_delay );
            virtual void show( const std::function<void()>& callback, short z_index );
        
        public:
            Node *_parent;
            bool _isActive;
            Sprite *_background;
    };
}

#endif
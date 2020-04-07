#ifndef __WINDOW_BASE_HH
#define __WINDOW_BASE_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"

USING_NS_CC;

namespace d_lan {

    class WindowBase : public Layer
    {
        public:
            void init( const Vec2& coords, short z_index );
            void set_background( std::string bg, float opacity );
            void hide( const std::function<void()>& callback );
            void show( const std::function<void()>& callback );
            void activate( void );
            void deactivate( void );
        
        private:
            Node *_parent;
            bool _isActive;
    };
}

#endif
#ifndef __PLAYER_WINDOW_HH
#define __PLAYER_WINDOW_HH 1

#include <iostream>
#include "../../../cocos2d/cocos/cocos2d.h"
#include "../../Window/WindowBase.h"

USING_NS_CC;

namespace d_lan {

    class PlayerWindow : public WindowBase
    {
        public:
            static PlayerWindow * create( void );
        
        private:
            
    };
}

#endif
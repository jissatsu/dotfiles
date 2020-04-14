#ifndef __D_LAN_DIALOGUE_PANEL_PROFILE_VIEW_HH
#define __D_LAN_DIALOGUE_PANEL_PROFILE_VIEW_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../Window/WindowBase.h"

namespace d_lan {
    namespace Dialogue {
        class Panel : public d_lan::WindowBase {
            public:
                static Panel * create( Node *node, std::string type, std::string bg, const Vec2& coords, short z_index );
                void set_type( std::string type );
                bool init( std::string type, std::string bg, const Vec2& coords );
        };
    }
}

#endif
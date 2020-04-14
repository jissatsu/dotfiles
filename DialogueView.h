#ifndef __DLAN_DIALOGUE_VIEW_HH
#define __DLAN_DIALOGUE_VIEW_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../Window/WindowBase.h"
#include "Panel.h"

USING_NS_CC;

#ifndef ITEM_FONT
    #define ITEM_FONT "fonts/Marker Felt.ttf"
#endif

namespace d_lan {

    class DialogueView : public d_lan::WindowBase
    {
        public:
            DialogueView * create( Node *node, std::string bg, const Vec2& coords, short z_index );
            void add_panel( std::string type, std::string bg, const Vec2& coords, short z_index );
            d_lan::Dialogue::Panel get_panel( std::string type );
            void set_portrait( std::string img, const Vec2& coords, short z_index );
            void set_name( std::string name, std::string font, float size, short z_index );

        protected:
            Sprite *_portrait;
            Label *_name;
            std::vector<d_lan::Dialogue::PanelProfile *> _panels;
    };
}

#endif
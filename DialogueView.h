#ifndef __DLAN_DIALOGUE_VIEW_HH
#define __DLAN_DIALOGUE_VIEW_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../Window/WindowBase.h"

USING_NS_CC;

#ifndef ITEM_FONT
    #define ITEM_FONT "fonts/Marker Felt.ttf"
#endif

namespace d_lan {

    class DialogueView : public d_lan::WindowBase
    {
        public:
            static DialogueView * create( Node *parent, std::string bg, const Vec2& coords, float opacity );
            void label( std::string name, std::string text, std::string font, float size, const Vec2& coords, float w, float h );
            void img( std::string name, std::string img, const Vec2& coords, short z_index );

        protected:
            Sprite *_portrait;
            Label *_name;
            bool wrap;
    };
}

#endif
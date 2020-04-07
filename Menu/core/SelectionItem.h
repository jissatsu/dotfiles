#ifndef __SELECTION_ITEM_HH
#define __SELECTION_ITEM_HH 1

#include <iostream>
#include "ItemState.h"
#include "../../../cocos2d/cocos/cocos2d.h"

USING_NS_CC;

#define ITEM_FONT "fonts/Marker Felt.ttf"

namespace d_lan {

    class SelectionItem
    {
        public:
            SelectionItem( const std::string& text, const std::function<void()>& func, bool enabled, short index );
            ~SelectionItem();
            void set_handler( const std::function<void()>& func );
            CallFunc * get_handler( void ) const;
            void set_index( short index );
            void set_text( const std::string& text );
            void add_label( std::string name, std::string text, float size, const Vec2& coords );
            void add_icon( std::string name, std::string img, const Vec2& coords );
            short get_index( void ) const;
            Layer * get_layer( void ) const;
            const ItemState get_state( void ) const;
            const std::string get_text( void ) const;
        
        public:
            Layer *_layer;
            Label *_label;
            bool _enabled;
            short _index;
            CallFunc *_handler;
    };

}
#endif

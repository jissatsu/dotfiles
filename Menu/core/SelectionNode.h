#ifndef __SELECTION_NODE_HH
#define __SELECTION_NODE_HH 1

#include <iostream>
#include "../../../cocos2d/cocos/cocos2d.h"
#include "SelectionItem.h"

USING_NS_CC;

namespace d_lan {

    class SelectionNode
    {
        public:
            SelectionNode( Node *parent, const Vec2& coords, short z_index );
            ~SelectionNode();
            void set_background( std::string bg, float opacity );
            void add( std::shared_ptr<d_lan::SelectionItem> item, short z_index );
            Layer * get_layer( void ) const;
            std::vector<std::shared_ptr<d_lan::SelectionItem>> items( void ) const;
            std::shared_ptr<d_lan::SelectionItem> get( const ItemState state ) const;
            int size( void ) const;
        public:
            Layer *_layer;
            Sprite *_background;
            std::vector<std::shared_ptr<d_lan::SelectionItem>> _items;
    };
}

#endif

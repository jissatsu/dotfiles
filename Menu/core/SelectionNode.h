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
            SelectionNode( Node *parent, std::string bg, const Vec2& coords );
            ~SelectionNode();
            void add( std::shared_ptr<d_lan::SelectionItem> item );
            Layer * get_layer( void ) const;
            std::vector<std::shared_ptr<d_lan::SelectionItem>> items( void ) const;
            int size( void ) const;
        public:
            Layer *_layer;
            Sprite *_background;
            std::vector<std::shared_ptr<d_lan::SelectionItem>> _items;
    };
}

#endif

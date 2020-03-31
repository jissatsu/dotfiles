#ifndef __OPTIONN_HH
#define __OPTIONN_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"

USING_NS_CC;

#define ITEM_FONT "fonts/Marker Felt.ttf"

namespace d_lan {
    
    class Option
    {
        public:
            Option( Node *node, const std::string& text, const std::function<void()>& func, bool enabled, short index );
            ~Option();
            void SetHandler( const std::function<void()>& func );
            CallFunc * GetHandler( void ) const;
            void SetIndex( short index );
            short GetIndex( void ) const;
            Layer * getLayer( void ) const;
            const std::string GetState( void ) const;

        public:
            Layer *_layer;
            Label *_label;
            bool _enabled;
            short _index;
            CallFunc *_handler;
    };
}

#endif
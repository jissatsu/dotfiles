#ifndef __ABSTRACT_MENU_HH
#define __ABSTRACT_MENU_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../../cocos2d/cocos/audio/include/AudioEngine.h"
#include "../Util.h"
#include "Cursor.h"
#include "Option.h"

USING_NS_CC;

#define ITEM_FONT "fonts/Marker Felt.ttf"

using namespace std::placeholders;

class AbstractMenu : public Layer
{
    public:
        virtual bool init( Node *parent );
        void lock_state( std::string state );
        void prev( void );
        void next( void );
        void confirm( void );
        short next_enabled( short *index, char ch ) const;
        virtual void show( void );
        virtual void hide( void );
        virtual void add_option( const std::string& text, const std::function<void()>& func, bool enabled, short index );
        d_lan::Option * GetSelection( void ) const;
    
    public:
        Size vsize;
        Vec2 origin;
        Node *_parent;
        std::shared_ptr<Cursor> _cursor;
        short _current;
        std::string _lock_state;
        std::vector<std::shared_ptr<d_lan::Option>> _selections;
};

#endif
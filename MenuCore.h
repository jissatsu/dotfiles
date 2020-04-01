#ifndef __ABSTRACT_MENU_HH
#define __ABSTRACT_MENU_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"
#include "Cursor.h"
#include "Option.h"
#include "OptionListView.h"

USING_NS_CC;

#define ITEM_FONT "fonts/Marker Felt.ttf"

using namespace std::placeholders;

class MenuCore : public Layer
{
    public:
        virtual bool init( Node *parent );
        void lock_state( std::string state );
        void prev( void );
        void next( void );
        void confirm( void );
        void update_cursor( std::shared_ptr<d_lan::Option> selection );
        short next_enabled( short *index, char ch ) const;
        virtual void setActive( bool active, const std::function<void()>& callback );
        virtual void add_option( const std::string& text, const std::function<void()>& func, bool enabled, short index );
        virtual void clearSelections( void );
        const std::string& getLockState( void ) const;
        d_lan::Option * GetSelection( void ) const;
    
    public:
        bool _isActive;
        Size vsize;
        Vec2 origin;
        Node *_parent;
        std::shared_ptr<Cursor> _cursor;
        std::shared_ptr<OptionListView> _listView;
        short _current;
        std::string _lock_state;
        std::vector<std::shared_ptr<d_lan::Option>> _selections;
};

#endif
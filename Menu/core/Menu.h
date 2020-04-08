#ifndef __D_LAN_MENU_CORE_HH
#define __D_LAN_MENU_CORE_HH 1

#include <iostream>
#include "../../../cocos2d/cocos/cocos2d.h"
#include "../Cursor.h"
#include "SelectionNode.h"
#include "../../Window/WindowBase.h"

USING_NS_CC;

#define ITEM_FONT "fonts/Marker Felt.ttf"

using namespace std::placeholders;

namespace d_lan {
    class Menu : public d_lan::WindowBase
    {
        public:
            //virtual bool init_base( Node *parent, const Vec2& coords, short z_index ) override;
            void lock_state( std::string state );
            virtual void prev( void );
            virtual void next( void );
            virtual void confirm( void );
            virtual void selection_next( void );
            virtual void selection_prev( void );
            void update_cursor( std::shared_ptr<d_lan::SelectionItem> selection_item );
            short next_enabled( short *index, char ch ) const;
            virtual void add_option( std::shared_ptr<d_lan::SelectionNode> node, const std::string& text, const std::function<void()>& func, bool enabled, short index );
            //virtual void clear( void );
            const std::string& getLockState( void ) const;
            std::shared_ptr<d_lan::SelectionNode> selection_node( short index ) const;
        
        public:
            Size vsize;
            Vec2 origin;
            std::shared_ptr<Cursor> _cursor;
            short _current;
            short _selected_node;
            std::string _lock_state;
            std::vector<std::shared_ptr<d_lan::SelectionNode>> _selection_nodes;
    };
}

#endif

#ifndef __ABSTRACT_MENU_HH
#define __ABSTRACT_MENU_HH 1

#include <iostream>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../../cocos2d/cocos/audio/include/AudioEngine.h"
#include "../CallbackManager/CallbackManager.h"
#include "../Util.h"
#include "../Item.h"
#include "Cursor.h"

USING_NS_CC;

#define ITEM_FONT "fonts/Marker Felt.ttf"

using namespace std::placeholders;

class AbstractMenu : public Node
{
    public:
        bool init( Node *parent );
        void lock_state( std::string state );
        void prev( void );
        void next( void );
        void confirm( void );
        short next_enabled( short *index, char ch ) const;
        virtual void show( void );
        virtual void hide( void );
        virtual void init_sounds( void );
        virtual bool cursor( void );
        virtual bool init_bg( void );
        virtual void add_option( std::string text, float y_offset, 
                          bool enabled, const std::function<void()>& func );
        d_lan::MenuItem * GetSelection( void ) const;
    
    private:
        class Option {
            Option( const std::string& text, const std::function<void()>& func, bool enabled );
            ~Option();
            void SetHandler( const std::function<void()>& func );

            public:
                Layer *_layer;
                Label *_label;
                bool _enabled;
                CallFunc *_handler;
        };
    
    public:
        Size vsize;
        Vec2 origin;
        Node *_parent;
        Cursor *_cursor;
        short _current;
        short items_count;
        Sprite *_cmd_container;
        std::string _lock_state;
        std::map<std::string, std::string> _sounds;
        std::vector<d_lan::MenuItem *> _selections;
};

#endif
#ifndef __QUICK_MENU_HH
#define __QUICK_MENU_HH 1

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

class QuickMenu : public Node
{
    public:
        bool init( std::string type, std::string name, Node *parent );
        bool is_valid_type( std::string type );
        void lock_state( std::string state );
        void prev( void );
        void next( void );
        void select( void );
        short next_enabled( short *index, char ch );
        short first_enabled( void );
        virtual void show( void );
        virtual void hide( void );
        virtual void init_sounds( void );
        virtual bool cursor( void );
        virtual bool init_bg( void );
        virtual void add_menu_items( void );
        virtual void add_menu_item( std::string text, float size, float y_offset, 
                          bool enabled, const std::function<void()> func );
        virtual void add_callback( std::string name, const std::function<void()> func );
        Item * selected_item( void );
        std::function<void()> init_callbacks;
    
    public:
        Size vsize;
        Vec2 origin;
        Node *_parent;
        Cursor *_cursor;
        short item_index;
        short items_count;
        std::string _type;
        Sprite *_cmd_container;
        std::string _lock_state;
        CallbackManager *_callback_manager;
        std::map<std::string, std::function<void()>> _callbacks;
        std::map<std::string, std::string> _sounds;
        std::vector<Item *> _menu_items;
};

#endif
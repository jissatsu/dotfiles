#include "QuickMenu.h"

bool QuickMenu::init( std::string type, std::string name, Node *parent )
{
    if ( !is_valid_type( type ) || name == "" ) {
        return false;
    }
    this->_parent = parent;
    this->_type   = type;
    this->_callback_manager = CallbackManager::create();
    this->_callback_manager->retain();
    this->origin  = Director::getInstance()->getVisibleOrigin();
    this->vsize   = Director::getInstance()->getVisibleSize();
    this->setName( name );
    
    if ( !this->init_bg() ) {
        return false;
    }

    this->init_sounds();
    this->init_callbacks();
    this->add_menu_items();

    this->items_count = this->_menu_items.size();
    this->item_index  = this->first_enabled();
    return true;
}

void QuickMenu::init_sounds( void )
{
    
}

bool QuickMenu::cursor( void )
{
    
}

bool QuickMenu::init_bg( void )
{
    
}

void QuickMenu::add_callback( std::string name, const std::function<void()> func )
{
    this->_callbacks[name] = func;
}

void QuickMenu::add_menu_items( void )
{
    
}

void QuickMenu::add_menu_item( std::string text, float size, float y_offset, 
                            bool enabled, const std::function<void()> func )
{
    
}

void QuickMenu::show( void )
{
    if ( this->getParent() == nullptr ) {
        this->_parent->addChild( this );
        this->cursor();
    }
}

void QuickMenu::hide( void )
{
    this->removeFromParent();
}

void QuickMenu::lock_state( std::string state )
{
    if ( state != "locked" && state != "unlocked" ) {
        this->_lock_state = "locked";
        return;
    }
    this->_lock_state = state;
}

void QuickMenu::next( void )
{
    if ( this->_lock_state != "locked" ) {
        if ( this->item_index < this->items_count - 1 ) {
            this->_cursor->setPosition(
                Vec2(
                    this->_cursor->getPosition().x,
                    this->_menu_items.at( next_enabled( &this->item_index, 'n' ) )->getPosition().y
                )
            );
            AudioEngine::play2d( this->_sounds["cursor-move"] );
        }
    }
}

void QuickMenu::prev( void )
{
    if ( this->_lock_state != "locked" ) {
        if ( this->item_index > 0 ) {
            this->_cursor->setPosition(
                Vec2(
                    this->_cursor->getPosition().x,
                    this->_menu_items.at( next_enabled( &this->item_index, 'p' ) )->getPosition().y
                )
            );
            AudioEngine::play2d( this->_sounds["cursor-move"] );
        }
    }
}

short QuickMenu::next_enabled( short *index, char ch )
{
    short accum = (ch == 'n') ? 1 : -1;
    short temp = *index;

    for ( auto item : this->_menu_items ) {
        temp += accum ;
        if ( temp == 0 || temp == this->items_count - 1 ) {
            return (this->_menu_items.at( temp )->get_state() == "enabled") ? (*index = temp) : *index ;
        }
        if ( this->_menu_items.at( temp )->get_state() == "enabled" ) {
            return (*index = temp);
        }
    }
}

void QuickMenu::select( void )
{
    if ( this->_lock_state != "locked" ) {
        AudioEngine::play2d( this->_sounds["cursor-select"] );
        this->_callback_manager->call( this->selected_item()->getName() );
    }
}

short QuickMenu::first_enabled( void )
{
    short i = 0;
    for ( auto item : this->_menu_items ) {
        if ( item->get_state() == "enabled" ) {
            return i;
        }
        i++;
    }
    return i;
}

Item * QuickMenu::selected_item( void )
{
    return this->_menu_items.at( this->item_index );
}

bool QuickMenu::is_valid_type( std::string type )
{
    if ( type != "menu" && type != "submenu" )
        return false;
    return true;
}
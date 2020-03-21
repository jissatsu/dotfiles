#include "PauseMenu.h"

PauseMenu * PauseMenu::create( void )
{
    PauseMenu *menu = new PauseMenu();
    if ( menu ) {
        menu->autorelease();
        return menu;
    }
    CC_SAFE_DELETE( menu );
    return nullptr;
}

bool PauseMenu::init_layout( void )
{
    this->_cmd_container = Sprite::create( "../Resources/tiles/text_boxes/menu/mmenu2-trs.png" );
    if ( !this->_cmd_container ) {
        return false;
    }
    this->_cmd_container->setAnchorPoint(
        Vec2(
            0.5f, 0.5f
        )
    );
    this->_cmd_container->setPosition(
        Vec2(
            this->origin.x + this->vsize.width  / 2,
            this->origin.y + this->vsize.height / 2
        )
    );
    this->addChild( this->_cmd_container );
    return true;
}

/* initialize menu cursor */
bool PauseMenu::init_cursor( void )
{
    this->_cursor = Cursor::create(
        this->_cmd_container,
        this->_menu_items, "../Resources/tiles/text_boxes/menu/za.png"
    );
    if ( !this->_cursor ) {
        return false;
    }
    this->_cursor->setScaleX( 0.7f );
    this->_cursor->setScaleY( 0.9f );
    this->_cursor->init_sound( Util::get_sounds()["cursor-mv"] );
    this->_cursor->animate( 0.5f );
    return true;
}

void PauseMenu::add_command( std::string text, float size, float y_offset, 
                             bool enabled, const std::function<void()> func )
{
    auto label = Label::createWithTTF( text, ITEM_FONT, size );
    auto item  = static_cast<Item*>( label );
    item->setAnchorPoint(
        Vec2(
            0.5f, 0.5f
        )
    );
    item->setPosition(
        Vec2(
            this->_cmd_container->getContentSize().width / 2,
            this->_cmd_container->getContentSize().height - item->getContentSize().height - y_offset - 4.4f
        )
    );
    if ( enabled ) {
        item->set_enabled();
    } else {
        item->set_disabled();
        item->setTextColor( Color4B( 89, 89, 89, 255 ) );
    }
    if ( func != 0 ) {
        this->_callback_manager->push_call( text, func );
    }
    item->setName( text );
    this->_cmd_container->addChild( label, 2 );
    this->_menu_items.push_back( item );
}

/* initialize the commands */
void PauseMenu::add_commands( void )
{
    add_command( "Resume",
        9.5f, 5.0f, true, CC_CALLBACK_0( PauseMenu::Die, this ) );

    add_command( "Options",
        9.5f, item_height(0) + 8.0f, true, 0 );

    add_command( "Save",
        9.5f, (item_height(1) + 5.0f) * 2, true, CC_CALLBACK_0( PauseMenu::Die, this ) );

    add_command( "Game End",
        9.5f, (item_height(2) + 4.0f) * 3, true, CC_CALLBACK_0( PauseMenu::Die, this ) );
}

/*
 * Get height of item
 */
float PauseMenu::item_height( short index )
{
    return this->_menu_items.at(index)->getContentSize().height;
}

void PauseMenu::Die( void )
{
    Director::getInstance()->end();
}
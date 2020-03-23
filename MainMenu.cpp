#include "MainMenu.h"

MainMenu * MainMenu::create( void )
{
    MainMenu *menu = new MainMenu();
    if ( menu ) {
        menu->autorelease();
        return menu;
    }
    CC_SAFE_DELETE( menu );
    return nullptr;
}

bool MainMenu::init_layout( void )
{
    this->_cmd_container = Sprite::create( "../Resources/tiles/text_boxes/menu/mainmenu2.png" );
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
            (this->origin.y - 70) + this->vsize.height / 2
        )
    );
    this->addChild( this->_cmd_container );
    return true;
}

/* initialize menu cursor */
bool MainMenu::init_cursor( void )
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
    this->_cursor->animate( 0.2f );
    return true;
}

/**/
void MainMenu::add_command( std::string text, float size, float y_offset, 
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

/*
 * initialize the commands
 */
void MainMenu::add_commands( void )
{
    add_command( "New Game",
        8.0f, 0.0f, true, this->_callbacks["New Game"] );

    add_command( "Load",
        8.0f, (item_height(0) + 3.5f), false, this->_callbacks["Load"] );

    add_command( "End",
        8.0f, (item_height(1) + 3.0f) * 2, true, this->_callbacks["End"] );
}

void MainMenu::init_sounds( void )
{
    this->_sounds["cursor-move"]   = Util::get_sounds()["cursor-mv"];
    this->_sounds["cursor-select"] = Util::get_sounds()["cursor-sl"];
}

/*
 * Get height of item
 */
float MainMenu::item_height( short index )
{
    return this->_menu_items.at(index)->getContentSize().height;
}
#include "MainMenu.h"
using namespace rapidjson;

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

bool MainMenu::init_bg( void )
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
bool MainMenu::cursor( void )
{
    this->_cursor = Cursor::create(
        this->_cmd_container,
        Vec2( 39.5f, 37.0f ),
        "../Resources/tiles/text_boxes/menu/za.png"
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
void MainMenu::add_menu_item( std::string text, float size, float y_offset, 
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
    item->set_enabled( enabled );
    
    if ( func != 0 ) {
        this->_callback_manager->push_call( text, func );
    }
    item->setName( text );
    this->_cmd_container->addChild( label, 2 );
    this->_menu_items.push_back( item );

    /* const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document document;
    document.Parse(json);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;

    if ( document.IsObject() ) {
        std::cout << "yeah object!" << "\n";
    }

    if ( document["stars"].IsString() ) {
        std::cout << "yeah int!" << "\n";
    } */
}

/*
 * initialize the commands
 */
void MainMenu::add_menu_items( void )
{
    add_menu_item( "New Game", 8.0f,  0.0f, true,  this->_callbacks["New Game"] );
    add_menu_item( "Load",     8.0f, 12.5f, false, this->_callbacks["Load"] );
    add_menu_item( "End",      8.0f, 24.5f, true,  this->_callbacks["End"] );
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
#include "MainMenuScene.h"

MainMenuScene * MainMenuScene::create( CallbackManager *manager )
{
    MainMenuScene *scene = new MainMenuScene();
    if ( scene && scene->init( manager ) ) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE( scene );
    return nullptr;
}

bool MainMenuScene::init_background( std::string img, float z_index )
{
    this->bg = Sprite::create( img );
    if (this->bg == nullptr) {
        return false;
    }
    this->bg->setAnchorPoint(
        Vec2(
            0.5f, 0.5f
        )
    );
    this->bg->setPosition(
        Vec2(
            this->origin.x + this->vsize.width  / 2,
            this->origin.y - 5 + this->vsize.height / 2
        )
    );
    this->addChild( this->bg, z_index );
    return true;
}

bool MainMenuScene::init_menu_items( void )
{
    this->item_cnt = Sprite::create( "../Resources/tiles/text_boxes/menu/modified1.png" );
    if (this->item_cnt == nullptr) {
        return false;
    }
    this->item_cnt->setAnchorPoint(
        Vec2(
            0.5f, 0.5f
        )
    );
    this->item_cnt->setPosition(
        Vec2(
            this->origin.x + this->vsize.width  / 2,
            this->origin.y - 75 + this->vsize.height / 2
        )
    );
    
    add_menu_item(  this->item_cnt, "New game", ITEM_FONT, 8.0f, 2, 0.0f, true );
    add_menu_item(  this->item_cnt, "Load",     ITEM_FONT, 8.0f, 2,  item_height_at(0) + 3, false );
    add_menu_item(  this->item_cnt, "Exit",     ITEM_FONT, 8.0f, 2, (item_height_at(1) + 3) * 2, true );
    this->addChild( this->item_cnt, 1 );
    return true;
}

/*
 * Add selectable item
 */
void MainMenuScene::add_menu_item( Node *node, std::string text, std::string font,
                                   float size, short z_index, float y_offset,
                                   bool enabled )
{
    auto label = Label::createWithTTF( text, font, size );
    auto item  = static_cast<Item*>( label );
    item->setAnchorPoint(
        Vec2(
            0.5f, 0.5f
        )
    );
    float ini_y_offset = 5.0f;
    item->setPosition(
        Vec2(
            node->getContentSize().width / 2,
            node->getContentSize().height - item->getContentSize().height - y_offset - 4.4f
        )
    );
    if ( enabled ) {
        item->set_enabled();
    } else {
        item->set_disabled();
        item->setTextColor( Color4B( 89, 89, 89, 255 ) );
    }
    item->setName( text );
    node->addChild( label, z_index );
    this->menu_items.push_back( item );
    this->_callback_manager->push_call( text, CC_CALLBACK_0( MainMenuScene::Die, this ) );
}

/*
 * Initialize the menu cursor with sound/selectable items
 */
bool MainMenuScene::init_cursor( void )
{
    this->cursor = Cursor::create(
        this->item_cnt,
        this->menu_items,
        "../Resources/tiles/text_boxes/menu/cursorRect1.png"
    );
    
    if ( !this->cursor ) {
        return false;
    }
    this->cursor->init_sound(
        Util::get_sounds()["cursor-mv"]
    );
    this->cursor->animate( 0.5f );
    return true;
}

/*
 * Get height of item
 */
float MainMenuScene::item_height_at( short index )
{
    return this->menu_items.at(index)->getContentSize().height;
}

bool MainMenuScene::init( CallbackManager *manager )
{
    if ( !Scene::init() ) {
        return false;
    }
    if ( !manager ) {
        return false;
    }
    // glfwSwapInterval( 0 );
    this->_callback_manager = manager;
    this->_callback_manager->retain();
    this->vsize  = Director::getInstance()->getVisibleSize();
    this->origin = Director::getInstance()->getVisibleOrigin();

    if ( !this->init_background( "../Resources/tiles/bcnd/nagato-yuki.png", 0 ) ) {
        return false;
    }
    if ( !this->init_menu_items() ) {
        return false;
    }
    if ( !this->init_cursor() ) {
        return false;
    }
    init_keyboard( this, this->_eventDispatcher );
    AudioEngine::play2d(
        Util::get_sounds()["main-menu"],
        true,
        0.8f
    );
    return true;
}

void MainMenuScene::init_keyboard( Scene *scene, EventDispatcher *disp )
{
    auto listn = EventListenerKeyboard::create();
    listn->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch ( keyCode ) {
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                this->cursor->prev();
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                this->cursor->next();
                break;
            case EventKeyboard::KeyCode::KEY_ENTER:
                this->_callback_manager->call( this->cursor->selected_item()->getName() );
                break;
        }
    };
    disp->addEventListenerWithSceneGraphPriority( listn, scene );
}

void MainMenuScene::Die( void )
{
    this->_callback_manager->release_all();
    Director::getInstance()->end();
}
#include "HotelScene1.h"

HotelScene1 * HotelScene1::create( void )
{
    HotelScene1 *scene = new HotelScene1();
    if ( scene && scene->init() ) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE( scene );
    return nullptr;
}

bool HotelScene1::init_player( TMXTiledMap *tile_map )
{
    this->player = Player::create( 1.05f, tile_map );
    if ( this->player == nullptr ) {
        return false;
    }

    // this->has_spawn_point( false );
    this->player->set_direction( "down" );
    this->player->load_sprites( "left",  "../Resources/sprites/horror/pc4/a", ".png" );
    this->player->load_sprites( "right", "../Resources/sprites/horror/pc4/d", ".png" );
    this->player->load_sprites( "up",    "../Resources/sprites/horror/pc4/w", ".png" );
    this->player->load_sprites( "down",  "../Resources/sprites/horror/pc4/s", ".png" );

    Vec2 spawn = Util::get_spawn_point( tile_map );
    this->player->spawn_at(
        Vec2(
            spawn.x,
            spawn.y
        ),
        this->player->frames["down"].at(0)
    );
    return true;
}

bool HotelScene1::init_tilemap( std::string tmxmap )
{
    this->tile_map = TMXTiledMap::create( tmxmap );
    if ( this->tile_map == nullptr ) {
        return false;
    }
    Director *director = Director::getInstance();
    this->tile_map->setAnchorPoint(
        Vec2( 0.5, 0.5 )
    );
    this->tile_map->setPosition(
        Vec2(
            director->getVisibleOrigin().x + director->getVisibleSize().width  / 2,
            director->getVisibleOrigin().y + director->getVisibleSize().height / 2
        )
    );
    this->addChild( this->tile_map );
    return true;
}

bool HotelScene1::init( void )
{
    if ( !Scene::init() ) {
        return false;
    }
    if ( !this->init_tilemap( "../Resources/tmx/untitled6-packer.tmx" ) ) {
        return false;
    }
    if ( !this->init_player( this->tile_map ) ) {
        return false;
    }
    this->global_state = INGAME;
    Director::getInstance()->setProjection( Director::Projection::_2D );
    this->init_keyboard( this, this->_eventDispatcher );
    this->scheduleUpdate();
    return true;
}

void HotelScene1::init_keyboard( Scene *scene, EventDispatcher *disp )
{
    auto listn = EventListenerKeyboard::create();
    listn->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
        if( std::find(this->_pressed.begin(), this->_pressed.end(), keyCode) == this->_pressed.end() ) {
            this->_pressed.push_back( keyCode );
        }
        if ( this->global_state == INGAME ) {
            if ( this->dir4() ) {
                this->player->move( this->keymap[this->_pressed.back()] );
                this->player->state == MOV;
            }
            else if ( this->_pause() ) {
                this->release_data();
            }
        }
    };
    listn->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
        this->_pressed.erase( std::remove(this->_pressed.begin(), this->_pressed.end(), keyCode), this->_pressed.end() );
        if ( this->global_state == INGAME ) {
            if ( !this->_pressed.empty() ) {
                if ( this->dir4() ) {
                    this->player->move( this->keymap[this->_pressed.back()] );
                    this->player->state == MOV;
                }
            } else {
                this->player->idle();
                this->player->state = IDLE;
            }
        }
    };
    disp->addEventListenerWithSceneGraphPriority( listn, this );
}

bool HotelScene1::dir4( void )
{
    auto key = this->_pressed.back();
    if ( key == EventKeyboard::KeyCode::KEY_UP_ARROW   || key == EventKeyboard::KeyCode::KEY_DOWN_ARROW || 
         key == EventKeyboard::KeyCode::KEY_LEFT_ARROW || key == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) {
        return true;
    }
    return false;
}

bool HotelScene1::_pause( void )
{
    auto key = this->_pressed.back();
    if ( key == EventKeyboard::KeyCode::KEY_ESCAPE ) {
        return true;
    }
    return false;
}

void HotelScene1::release_data( void )
{
    this->player->release_animations();
    Util::clear_all();
}

void HotelScene1::update( float delta )
{
    this->player->update_pos( delta );
}
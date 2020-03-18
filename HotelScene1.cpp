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
    this->player->load_sprites( "left",  "../Resources/sprites/horror/pc1/a", ".png" );
    this->player->load_sprites( "right", "../Resources/sprites/horror/pc1/d", ".png" );
    this->player->load_sprites( "up",    "../Resources/sprites/horror/pc1/w", ".png" );
    this->player->load_sprites( "down",  "../Resources/sprites/horror/pc1/s", ".png" );

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
        log( "Tilemap initialization error!\n" );
        return false;
    }
    if ( !this->init_player( this->tile_map ) ) {
        log( "Player initialization error!\n" );
        return false;
    }
    this->input = Input::create();
    if ( !this->input ) {
        log( "Input initialization error!\n" );
        return false;
    }
    Director::getInstance()->setProjection( Director::Projection::_2D );
    this->global_state = INGAME;
    this->input->retain();
    this->init_keyboard( this, this->_eventDispatcher );
    this->scheduleUpdate();
    return true;
}

void HotelScene1::init_keyboard( Scene *scene, EventDispatcher *disp )
{
    auto listn = EventListenerKeyboard::create();
    listn->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
        this->input->push( keyCode );
        if ( this->global_state == INGAME ) {
            if ( this->input->dir4() ) {
                this->player->move( this->input->map( keyCode ) );
            }
            else if ( this->input->_pause() ) {
                this->global_state = INMENU;
                this->player->idle();
            }
        }
        else if ( this->global_state == INMENU ) {
            if ( this->input->_pause() ) {
                this->global_state = INGAME;
            }
        }
        this->keyb_trigger(
            this->input->last_key(), "pressed"
        );
    };
    listn->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
        this->input->pop( keyCode );
        if ( this->global_state == INGAME ) {
            auto lkey = this->input->last_key();
            if ( lkey != EventKeyboard::KeyCode::KEY_NONE ) {
                if ( this->input->dir4() ) {
                    this->player->move( this->input->map( lkey ) );
                }
            } else {
                this->player->idle();
            }
        }
        this->keyb_trigger(
            this->input->last_key(), "released"
        );
    };
    disp->addEventListenerWithSceneGraphPriority( listn, this );
}

/*
 * toggle scene state
 */
void HotelScene1::scene_state( void )
{
    if ( this->global_state == INGAME ) {
        this->global_state = INMENU, this->player->idle(); return;
    }
    if ( this->global_state == INMENU ) {
        this->global_state = INGAME; return;
    }
}
/*
 * move the player
 */
void HotelScene1::move_player( std::string keystate )
{
    auto lkey = this->input->last_key();
    if ( this->global_state == INGAME ) {
        if ( keystate == "pressed" ) {
            if ( this->input->dir4() ) {
                this->player->move( this->input->map( lkey ) );
            }
            return;
        }
        if ( keystate == "released" ) {
            if ( lkey != EventKeyboard::KeyCode::KEY_NONE ) {
                if ( this->input->dir4() ) {
                    this->player->move( this->input->map( lkey ) );
                }
            } else {
                this->player->idle();
            }
        }
    }
}

void HotelScene1::keyb_trigger( EventKeyboard::KeyCode code, std::string keystate )
{
    if ( code != EventKeyboard::KeyCode::KEY_NONE ) {
        log( this->input->map( code ).c_str() );
    }
}

void HotelScene1::release_data( void )
{
    this->player->release_animations();
    this->input->release();
    Util::clear_all();
}

void HotelScene1::update( float delta )
{
    //this->menu->update();
    this->player->update_pos( delta );
}
#include "Player.h"

/* Create player instance and initialize its sprite frame cache */
Player * Player::create( float ani_speed, TMXTiledMap *tile_map, Node *spawn_node )
{
    Player *player = new Player();
    if ( player && spawn_node )
    {
        player->init( ani_speed, tile_map );
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE( player );
    return nullptr;
}

void Player::init( float ani_speed, TMXTiledMap *tile_map )
{
    this->x_velocity = 0;
    this->y_velocity = 0;
    this->ani_speed  = (ani_speed <= 0) ? 1.0f : ani_speed ;
    this->tile_map   = tile_map;
    this->cache      = SpriteFrameCache::getInstance();
}

std::string Player::get_file_name( std::string path )
{
    const size_t name_begin = path.find_last_of("\\/");
    if ( name_begin != std::string::npos ) {
        return path.erase(0, name_begin + 1);
    }
    return "";
}

void Player::load_sprites( std::string direction, std::string path, std::string ext )
{
    Files files = FileUtils::getInstance()->listFiles( path );
    // load plist files first
    for ( std::string file : files ) {
        if ( file.find( ".plist" ) != std::string::npos ) {
            std::string abs_path = path + "/" + this->get_file_name( file );
            this->cache->addSpriteFramesWithFile( abs_path );
        }
    }
    for ( std::string file : files ) {
        if ( file.find( ext ) != std::string::npos ) {
            SpriteFrame *frame = this->cache->getSpriteFrameByName( this->get_file_name( file ) );
            if ( frame ) {
                this->frames[direction].pushBack( frame );
            }
        }
    }
    this->init_animation( direction );
}

void Player::init_animation( std::string direction )
{
    Vector<SpriteFrame *> frames = this->frames[direction];
    Animation *animation = Animation::createWithSpriteFrames( frames, this->ani_speed / 8 );
    RepeatForever *repeat = RepeatForever::create(
        Animate::create( animation )
    );
    repeat->retain();
    this->animations[direction] = repeat;
}

/* set the initial position of the player from the origin point */
void Player::set_ini_position( float x, float y, SpriteFrame *init_frame )
{
    this->setPosition(
        Vec2(
            Director::getInstance()->getVisibleOrigin().x + x,
            Director::getInstance()->getVisibleOrigin().y + y
        )
    );
    this->xPos = x;
    this->yPos = y;
    this->setSpriteFrame( init_frame );
    this->setAnchorPoint( Vec2( 0.5, 0.5 ) );
}

void Player::_input( void )
{
    //if ( input.find("L") != input.end() ) {
        // this->x_velocity *= -1;
    //}
}

void Player::move( float delta )
{
    this->xPos += this->x_velocity * delta;
    this->yPos += this->y_velocity * delta;
    //this->runAction(
    //   this->animations[this->animation_state]
    //);
    this->setPosition( Vec2( this->xPos, this->yPos ) );
}
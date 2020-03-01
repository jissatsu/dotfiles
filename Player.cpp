#include "Player.h"

/* Create player instance and initialize its sprite frame cache */
Player * Player::create( float speed, Node *node )
{
    Player *player = new Player();
    if ( player && node )
    {
        player->cache = SpriteFrameCache::getInstance();
        player->init( speed );
        player->autorelease();
        return player;
    }
    return nullptr;
}

std::string Player::get_file_name( std::string path )
{
    const size_t name_begin = path.find_last_of("\\/");
    if ( name_begin != std::string::npos ) {
        return path.erase(0, name_begin + 1);
    }
    return "";
}

void Player::load_sprites( std::string direction, std::string path, std::string ext, short count )
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
    this->init_animations( direction );
}

void Player::init_animations( std::string direction )
{
    Vector<SpriteFrame *> frames = this->frames[direction];
    auto animation = Animation::createWithSpriteFrames( frames, 1.0f / 8 );
    auto repeat = RepeatForever::create(
        Animate::create( animation )
    );
    repeat->retain();
    this->animations[direction] = repeat;
}

void Player::init( float speed )
{
    this->x_velocity = 0;
    this->y_velocity = 0;

    if ( speed <= 0 )
        this->speed = 1.0f;
    else
        this->speed = speed;
    this->setAnchorPoint( Vec2( 0.5, 0.5 ) );
}

/* set the initial position of the player from the origin point */
void Player::set_position( float x, float y, SpriteFrame *init_frame )
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
}

void Player::_input( void )
{
    //if ( input.find("L") != input.end() ) {
    //    this->x_velocity -= this->speed;
    //}
}

void Player::move( float delta )
{
    this->xPos += this->x_velocity * delta;
    this->yPos += this->y_velocity * delta;
    this->setPosition( Vec2( this->xPos, this->yPos ) );
}
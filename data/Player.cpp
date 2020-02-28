#include "Player.h"

/* Create player instance and initialize its sprite frame cache */
Player * Player::create( float speed, const char *plist, Node *node )
{
    Player *player = new Player();
    if ( player && node )
    {
        player->initWithFile( "/home/killua/MyGamme/MyGame/Resources/sprites/doc/d/profd0.png" );
        player->cache = SpriteFrameCache::getInstance();
        player->init( speed );
        player->autorelease();
        node->addChild( player );
        return player;
    }
    return nullptr;
}

void Player::load_anim_frames( const char * direction, const char *format, int count )
{
    char str[100];
    for ( int i = 0 ; i < count ; i++ )
    {
        sprintf( str, format, i );
        this->frames[direction].pushBack( this->cache->getSpriteFrameByName( str ) );
    }
    return;
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
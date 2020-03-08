#include "Cursor.h"

Cursor * Cursor::create( Node *node, std::vector<Item *> items, std::string img )
{
    Cursor *cursor = new Cursor();
    if ( cursor && node && cursor->init( node, items, img ) )
    {
        cursor->autorelease();
        return cursor;
    }
    CC_SAFE_DELETE( cursor );
    return nullptr;
}

bool Cursor::init( Node *node, std::vector<Item *> items, std::string img )
{
    if ( !this->initWithFile( img ) ) {
        return false;
    }
    this->index = 0;
    this->items_count = items.size();
    this->items = items;
    auto enabled = this->get_first_enabled();
    this->setAnchorPoint(
        Vec2(
            0.5f, 0.5f
        )
    );
    this->setPosition(
        Vec2(
            node->getContentSize().width / 2,
            enabled->getPosition().y
        )
    );
    this->setScaleX( 0.7f );
    this->setOpacity( 50 );
    node->addChild( this, 1 );
    return true;
}

Item * Cursor::get_first_enabled( void )
{
    for ( auto item : this->items ) {
        if ( item->get_state() == "enabled" ) {
            return item;
        }
    }
    return nullptr;
}

short Cursor::next_enabled( short *index, char ch )
{
    short accum = (ch == 'n') ? 1 : -1;
    for ( auto item : this->items ) {
        *index += accum ;
        if ( this->items.at( *index )->get_state() == "enabled" ) {
            return *index;
        }
    }
}

void Cursor::next( void )
{
    if ( this->index < this->items_count - 1 ) {
        this->setPosition(
            Vec2(
                this->getPosition().x,
                this->items.at( next_enabled( &this->index, 'n' ) )->getPosition().y
            )
        );
        AudioEngine::play2d( this->sound );
    }
}

void Cursor::prev( void )
{
    if ( this->index > 0 ) {
        this->setPosition(
            Vec2(
                this->getPosition().x,
                this->items.at( next_enabled( &this->index, 'p' ) )->getPosition().y
            )
        );
        AudioEngine::play2d( this->sound );
    }
}

Item * Cursor::selected_item( void )
{
    return this->items.at( this->index );
}

void Cursor::init_sound( const char *soundfile )
{
    this->sound = soundfile;
}

void Cursor::animate( float tm )
{
    Vector<FiniteTimeAction*> actions;
    actions.pushBack( FadeTo::create( 0.3f, 225 ) );
    actions.pushBack( FadeTo::create( 0.3f,  50 ) );
    this->runAction( RepeatForever::create( Sequence::create( actions ) ) );
}
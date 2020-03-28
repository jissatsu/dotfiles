#include "AbstractMenu.h"

bool AbstractMenu::init( Node *parent ) {
    _parent = parent;
    if ( !init_bg() ) { return false; }
    init_sounds();
    return true;
}

void AbstractMenu::init_sounds( void )
{
    
}

bool AbstractMenu::cursor( void )
{
    
}

bool AbstractMenu::init_bg( void )
{
    
}

void AbstractMenu::add_option( std::string text, float y_offset, 
                            bool enabled, const std::function<void()>& func )
{
    
}

void AbstractMenu::lock_state( std::string state ) {
    if ( state != "locked" && state != "unlocked" ) {
        _lock_state = "locked";
        return;
    }
    _lock_state = state;
}

void AbstractMenu::next( void ) {
    if ( _lock_state != "locked" ) {
        if ( _current < _selections.size() - 1 ) {
            _cursor->setPosition(
                Vec2(
                    _cursor->getPosition().x,
                    _selections.at( next_enabled( &_current, 'n' ) )->getPosition().y
                )
            );
            AudioEngine::play2d( _sounds["cursor-move"] );
        }
    }
}

void AbstractMenu::prev( void ) {
    if ( _lock_state != "locked" ) {
        if ( _current > 0 ) {
            _cursor->setPosition(
                Vec2(
                    _cursor->getPosition().x,
                    _selections.at( next_enabled( &_current, 'p' ) )->getPosition().y
                )
            );
            AudioEngine::play2d( _sounds["cursor-move"] );
        }
    }
}

short AbstractMenu::next_enabled( short *index, char ch ) const {
    short accum = (ch == 'n') ? 1 : -1;
    short temp = *index;

    for ( auto selection : _selections ) {
        temp += accum ;
        if ( temp == 0 || temp == _selections.size() - 1 ) {
            return (_selections.at( temp )->GetState() == "enabled") ? (*index = temp) : *index ;
        }
        if ( _selections.at( temp )->GetState() == "enabled" ) {
            return (*index = temp);
        }
    }
}

void AbstractMenu::confirm( void ) {
    if ( _lock_state != "locked" ) {
        AudioEngine::play2d( _sounds["cursor-select"] );
        auto selection = GetSelection();
        auto handler  = selection->GetHandler();
        if ( handler ) {
            handler->execute();
        }
    }
}

void AbstractMenu::show( void ) {
    if ( getParent() == nullptr ) {
        _parent->addChild( this );
        for ( auto selection : _selections ) {
            if ( selection->GetState() == "enabled" ) { _current = selection->GetIndex();
                break;
            }
        }
        cursor();
    }
}

void AbstractMenu::hide( void ) {
    if ( getParent() != nullptr ) {
        removeFromParent();
    }
}

d_lan::MenuItem * AbstractMenu::GetSelection( void ) const {
    return _selections.at( _current );
}
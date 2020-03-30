#include "AbstractMenu.h"

bool AbstractMenu::init( Node *parent ) {

}
bool AbstractMenu::cursor( void ) {

}
void AbstractMenu::add_option( const std::string& text, const std::function<void()>& func, bool enabled, short index ) {
    
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
                    _selections.at( next_enabled( &_current, 'n' ) )->getLayer()->getPosition().y
                )
            );
        }
    }
}

void AbstractMenu::prev( void ) {
    if ( _lock_state != "locked" ) {
        if ( _current > 0 ) {
            _cursor->setPosition(
                Vec2(
                    _cursor->getPosition().x,
                    _selections.at( next_enabled( &_current, 'p' ) )->getLayer()->getPosition().y
                )
            );
        }
    }
}

short AbstractMenu::next_enabled( short *index, char ch ) const {
    short accum = (ch == 'n') ? 1 : -1;
    short temp = *index;

    for ( auto& selection : _selections ) {
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
        for ( auto& selection : _selections ) {
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

d_lan::Option * AbstractMenu::GetSelection( void ) const {
    return _selections.at( _current ).get();
}
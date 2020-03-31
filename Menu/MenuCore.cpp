#include "MenuCore.h"

bool MenuCore::init( Node *parent ) {

}

void MenuCore::add_option( const std::string& text, const std::function<void()>& func, bool enabled, short index ) {
    
}

void MenuCore::lock_state( std::string state ) {
    if ( state != "locked" && state != "unlocked" ) {
        _lock_state = "locked";
        return;
    }
    _lock_state = state;
}

const std::string& MenuCore::getLockState( void ) const {
    return _lock_state;
}

void MenuCore::next( void ) {
    if ( _lock_state != "locked" ) {
        if ( _current < _selections.size() - 1 ) {
            auto cursor = _cursor->GetLayer();
            cursor->setPosition(
                Vec2(
                    cursor->getPosition().x,
                    _selections.at( next_enabled( &_current, 'n' ) )->getLayer()->getPosition().y
                )
            );
        }
    }
}

void MenuCore::prev( void ) {
    if ( _lock_state != "locked" ) {
        if ( _current > 0 ) {
            auto cursor = _cursor->GetLayer();
            cursor->setPosition(
                Vec2(
                    cursor->getPosition().x,
                    _selections.at( next_enabled( &_current, 'p' ) )->getLayer()->getPosition().y
                )
            );
        }
    }
}

short MenuCore::next_enabled( short *index, char ch ) const {
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

void MenuCore::confirm( void ) {
    if ( _lock_state != "locked" ) {
        auto selection = GetSelection();
        auto handler  = selection->GetHandler();
        if ( handler ) {
            handler->execute();
        }
    }
}

void MenuCore::setActive( bool active, const std::function<void()>& handler ) {
    switch ( active ) {
        case true:
            if ( !_isActive ) {
                _parent->addChild( this );
                for ( auto& selection : _selections ) {
                    if ( selection->GetState() == "enabled" ) { _current = selection->GetIndex();
                        break;
                    }
                }
                if ( handler ) {
                    handler();
                }
            }
            break;
        default:
            if ( _isActive ) {
                if ( getParent() != nullptr ) {
                    if ( handler ) {
                        handler();
                    }
                    removeFromParent();
                }
            }
            break;
    }
    _isActive = active;
}

d_lan::Option * MenuCore::GetSelection( void ) const {
    return _selections.at( _current ).get();
}

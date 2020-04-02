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
        short tmp = _current;
        short N = _selections.size();
        for ( auto& selection : _selections ) {
            if ( _selections[tmp]->GetState() == "enabled" && tmp != _current ) {
                _current = tmp;
                break;
            }
            tmp = (tmp + 1) % N;
        }
        update_cursor( _selections[_current] );
    }
}

void MenuCore::prev( void ) {
    if ( _lock_state != "locked" ) {
        short tmp = _current;
        short N = _selections.size() - 1;
        for ( auto& selection : _selections ) {
            if ( _selections[tmp]->GetState() == "enabled" && tmp != _current ) {
                _current = tmp;
                break;
            }
            tmp = tmp - 1;
            if ( tmp < 0 ) { tmp = N; }
        }
        update_cursor( _selections[_current] );
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

void MenuCore::update_cursor( std::shared_ptr<d_lan::Option> selection ) {
    auto cursor = _cursor->GetLayer();
    cursor->setPosition(
        Vec2(
            cursor->getPosition().x,
            selection->getLayer()->getPosition().y
        )
    );
}

void MenuCore::clearSelections( void ) {
    for ( auto& selection : _selections ) {
        _listView->getLayer()->removeChild( selection->getLayer() );
    }
}

d_lan::Option * MenuCore::GetSelection( void ) const {
    return _selections[_current].get();
}

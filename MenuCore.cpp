#include "Menu.h"

bool Menu::init( Node *parent ) {

}

void Menu::add_option( const std::string& text, const std::function<void()>& func, bool enabled, short index ) {
    
}

void Menu::lock_state( std::string state ) {
    if ( state != "locked" && state != "unlocked" ) {
        _lock_state = "locked";
        return;
    }
    _lock_state = state;
}

const std::string& Menu::getLockState( void ) const {
    return _lock_state;
}

void Menu::next( void ) {
    if ( _lock_state != "locked" ) {
        short tmp = _current;
        auto selection = _selections[_selected];
        for ( auto& item : selection ) {
            if ( selection[tmp]->state == ItemState::ENABLED && tmp != _current ) {
                _current = tmp;
                break;
            }
            tmp = (tmp + 1) % selection.size();
        }
        update_cursor( selection[_current] );
    }
}

void Menu::prev( void ) {
    if ( _lock_state != "locked" ) {
        short tmp = _current;
        auto selection = _selections[_selected];
        for ( auto& item : selection ) {
            if ( selection[tmp]->state == ItemState::ENABLED && tmp != _current ) {
                _current = tmp;
                break;
            }
            tmp = tmp - 1;
            if ( tmp < 0 ) {
                tmp = selection.size() - 1;
            }
        }
        update_cursor( selection[_current] );
    }
}

void Menu::selection_next( void ) {
    if ( _selection >= 0 && _selection < _selections.size() - 1 ) {
        _selection += 1;
    }
}

void Menu::selection_prev( void ) {
    if ( _selection > 0 ) {
        _selection -= 1;
    }
}

void Menu::confirm( void ) {
    if ( _lock_state != "locked" ) {
        auto selection = GetSelection();
        auto handler  = selection->GetHandler();
        if ( handler ) {
            handler->execute();
        }
    }
}

void Menu::setActive( bool active, const std::function<void()>& handler ) {
    switch ( active ) {
        case true:
            if ( !_isActive ) {
                _parent->addChild( this );
                for ( auto& item : _layout[_selection] ) {
                    if ( item->state == ItemState::ENABLED ) { _current = selection->GetIndex();
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

void Menu::update_cursor( std::shared_ptr<d_lan::Option> selection ) {
    auto cursor = _cursor->GetLayer();
    cursor->setPosition(
        Vec2(
            cursor->getPosition().x,
            selection->getLayer()->getPosition().y
        )
    );
}

void Menu::clearSelections( void ) {
    for ( auto& selection : _selections ) {
        _listView->getLayer()->removeChild( selection->getLayer() );
    }
}

d_lan::Option * Menu::GetSelection( void ) const {
    return _selections[_current].get();
}

#include "Menu.h"

namespace d_lan {

    //bool Menu::init_base( Node *parent, const Vec2& coords, short z_index ){
        
    //}
    
    void Menu::add_option( std::shared_ptr<d_lan::SelectionNode> node, const std::string& text, const std::function<void()>& func, bool enabled, short index ) {
        
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
            auto selection = _selection_nodes[_selected_node]->items();
            for ( auto& item : selection ) {
                if ( selection[tmp]->get_state() == ItemState::ENABLED && tmp != _current ) {
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
            auto selection = _selection_nodes[_selected_node]->items();
            for ( auto& item : selection ) {
                if ( selection[tmp]->get_state() == ItemState::ENABLED && tmp != _current ) {
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
        auto nodes_count = _selection_nodes.size();
        if ( nodes_count > 1 ) {
            if ( _selected_node >= 0 && _selected_node < nodes_count - 1 ) {
                _selected_node += 1;
                update_cursor( _selection_nodes[_selected_node]->get( ItemState::ENABLED ) );
            }
        }
    }

    void Menu::selection_prev( void ) {
        if ( _selected_node > 0 ) {
            _selected_node -= 1;
            update_cursor( _selection_nodes[_selected_node]->get( ItemState::ENABLED ) );
        }
    }

    void Menu::confirm( void ) {
        if ( _lock_state != "locked" ) {
            auto selection = _selection_nodes[_selected_node]->items();
            auto handler  = selection[_current]->get_handler();
            if ( handler ) {
                handler->execute();
            }
        }
    }

    void Menu::update_cursor( std::shared_ptr<d_lan::SelectionItem> selection_item ) {
        auto cursor = _cursor->GetLayer();
        cursor->setPosition(
            Vec2(
                cursor->getPosition().x,
                selection_item->get_layer()->getPosition().y + 0.5f
            )
        );
    }
    
    std::shared_ptr<d_lan::SelectionNode> Menu::selection_node( short index ) const {
        auto nodes_count = _selection_nodes.size();
        if ( index < 0 || index >= nodes_count ) {
            return nullptr;
        }
        return _selection_nodes[index];
    }
}
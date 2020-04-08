#include "MainMenu.h"

MainMenu * MainMenu::create( Node *parent ) {
    MainMenu *menu = new MainMenu();
    if ( menu && menu->init_base( parent, {240.0f, 0.0f}, 2 ) ) {
        menu->set_background( UI::MainMenu::optionLV, 255.0f );
        menu->init();
        menu->autorelease();
        return menu;
    }
    CC_SAFE_DELETE( menu );
    return nullptr;
}

/* override of Menu::init() method */
bool MainMenu::init( void ) {
    // set the currently selected selection_node
    _selected_node = 0;

    auto node = std::shared_ptr<d_lan::SelectionNode>(new d_lan::SelectionNode(this, {0.0f, 0.0f}, 2));
    _selection_nodes.push_back( node );

    // initialize menu cursor
    _cursor = std::shared_ptr<Cursor>(
        new Cursor(_selection_nodes[_selected_node]->get_layer(), {0.5f, -36.5f}, 0.7f, 0.9f, UI::MainMenu::cursor));
    _cursor->pulse( 0.2f );
    return true;
}

/* override of WindowBase::show() method */
void MainMenu::show( const std::function<void()>& callback, short z_index ) {
    if ( !_isActive ) {
        _parent->addChild( this, z_index );
        if ( callback ) {
            callback();
        }
        auto item = _selection_nodes[_selected_node]->get( ItemState::ENABLED );
        _current = item->get_index();
    }
    _isActive = true;
}

/* Add a new selection item to the current selection node */
void MainMenu::add_option( std::shared_ptr<d_lan::SelectionNode> selection_node, const std::string& text, const std::function<void()>& func, bool enabled, short index ) {
    if ( selection_node == nullptr ) {
        throw std::range_error("No such selection node!\n");
    }
    auto item = new d_lan::SelectionItem(text, func, enabled, index);
    selection_node->add(
        std::move(
            std::shared_ptr<d_lan::SelectionItem>( item )
        ), 2
    );
    update( selection_node );
}

void MainMenu::update( std::shared_ptr<d_lan::SelectionNode> selection_node ) {
    auto cmd_pos = selection_node->get_layer()->getPosition();
    short count = selection_node->size();
    for ( auto& item : selection_node->items() ) {
        item->get_layer()->setPosition( Vec2( cmd_pos.x / 2, (12.3f * count) - 49 ) );
        count--;
    }
}

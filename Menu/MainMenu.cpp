#include "MainMenu.h"

MainMenu * MainMenu::create( Node *parent ) {
    MainMenu *menu = new MainMenu();
    if ( menu && menu->init( parent ) ) {
        menu->autorelease();
        return menu;
    }
    CC_SAFE_DELETE( menu );
    return nullptr;
}

bool MainMenu::init( Node *parent ) {
    if ( !parent ) {
        return false;
    }
    _parent = parent;
    _selected_node = 0;
    _selection_nodes.push_back(
        std::shared_ptr<d_lan::SelectionNode>(new d_lan::SelectionNode(this, UI::MainMenu::optionLV, {240.0f, 65.0f}))
    );
    // initialize menu cursor
    _cursor = std::shared_ptr<Cursor>(
        new Cursor(_selection_nodes[_selected_node]->get_layer(), {0.5f, -12.0f}, 0.7f, 0.9f, UI::MainMenu::cursor));
    _cursor->pulse( 0.2f );
    return true;
}

void MainMenu::add_option( std::shared_ptr<d_lan::SelectionNode> selection_node, const std::string& text, const std::function<void()>& func, bool enabled, short index ) {
    if ( selection_node == nullptr ) {
        throw std::range_error("No such selection node!\n");
    }
    selection_node->add(
        std::move( std::shared_ptr<d_lan::SelectionItem>(
            new d_lan::SelectionItem(
                selection_node->get_layer(), text, func, enabled, index
            )
        ))
    ); update( selection_node );
}

void MainMenu::update( std::shared_ptr<d_lan::SelectionNode> selection_node ) {
    auto cmd_pos = selection_node->get_layer()->getPosition();
    short count = selection_node->size();
    for ( auto& item : selection_node->items() ) {
        item->get_layer()->setPosition( Vec2( (cmd_pos.x - 240) / 2, (12.3f * count) - 24 ) );
        count--;
    }
}

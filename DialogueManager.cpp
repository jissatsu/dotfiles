#include "DialogueManager.h"

namespace d_lan {

    std::shared_ptr<DialogueManager> DialogueManager::_instance = nullptr;

    DialogueManager::DialogueManager( void )
        : _input(Input::instance()),
          _finished(false) {
    }

    DialogueManager::~DialogueManager(){};

    // Get an instance of the DialogueManager
    std::shared_ptr<DialogueManager> DialogueManager::instance( void ) {
        if ( !_instance ) {
            _instance = std::shared_ptr<DialogueManager>( new DialogueManager() );
        }
        return _instance;
    }

    // Create the dialogue view
    bool DialogueManager::set_view( Node *node, std::string bg, const Vec2& coords, float opacity ) {
        _dialogue_view = d_lan::DialogueView::create( node, bg, coords, opacity );
        if ( !_dialogue_view ) {
            return false;
        }
        _dialogue_view->retain();
        node->addChild( _dialogue_view, 2 );
        return true;
    }

    // Set the text character beep sound file
    std::shared_ptr<DialogueManager> DialogueManager::set_beep( std::string file ) {
        _beep_sound = file;
        return _instance;
    }

    // Set the text animation interval
    std::shared_ptr<DialogueManager> DialogueManager::set_timer( float timer ) {
        if ( timer <= 0.0f ) {
            _has_timer = false;
        } else {
            _timer = timer;
            _has_timer = true;
        }
        return _instance;
    }

    // Initialize the dialogue queue
    bool DialogueManager::init_dialogue( const std::string& json_file ) {
        Document json = json_util::json_decode( json_file );
        if ( !json.IsObject() ) {
            return false;
        }
        for ( const auto& dpage : json.GetObject() ) {
            _dialogue_queue.push( std::shared_ptr<d_lan::DialoguePage>( new d_lan::DialoguePage( dpage ) ) );
        }
        return true;
    }

    /**/
    void DialogueManager::show_next( void ) {
        auto current_page = _dialogue_queue.front();
        _message = current_page->get("text");
        _dialogue_view->getChildByName( "name" )->setText(current_page->get("name"));
        _dialogue_view->getChildByName( "text" )->setText("");
        _dialogue_view->getChildByName( "expression" )->setTexture(current_page->get("face"));
        _dialogue_queue.front().reset();
        _dialogue_queue.pop();
    }

    /**/
    bool DialogueManager::finished( void ) const {
        return _finished;
    }

    // Listen for ENTER key press and show next dialogue message (with expression and name)
    void DialogueManager::processInput( void ) {
        if ( _input->isPressed( EventKeyboard::KeyCode::KEY_ENTER ) ) {
            if ( !_dialogue_queue.size() > 0 ) {
                show_next();
            } else {
                _dialogue_view->setVisible( false );
                _finished = true;
            }
        }
    }

    void DialogueManager::update( float delta ) {
        if ( !_finished ) {
            if ( _text_ended ) {
                processInput();
            } else {
                
            }
        }
    }

    d_lan::DialogueView * DialogueManager::get_view( void ) const {
        return _dialogue_view;
    }
}
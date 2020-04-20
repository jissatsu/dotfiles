#include "DialogueManager.h"

namespace d_lan {

    std::shared_ptr<DialogueManager> DialogueManager::_instance = nullptr;

    DialogueManager::DialogueManager( void )
        : _input(Input::instance()) {
    }

    DialogueManager::~DialogueManager(){};

    /*
     * Get an instance of the DialogueManager
     */
    std::shared_ptr<DialogueManager> DialogueManager::instance( void ) {
        if ( !_instance ) {
            _instance = std::shared_ptr<DialogueManager>(
                new DialogueManager()
            );
        }
        return _instance;
    }

    /*
     * Create the dialogue view
     */
    bool DialogueManager::set_view( Node *node, std::string bg, const Vec2& coords, float opacity ) {
        _dialogue_view = d_lan::DialogueView::create( node, bg, coords, opacity );
        if ( !_dialogue_view ) {
            return false;
        }
        _dialogue_view->retain();
        node->addChild( _dialogue_view, 2 );
        return true;
    }

    d_lan::DialogueView * DialogueManager::get_view( void ) const {
        return _dialogue_view;
    }

    /*
     * Set the text character beep sound
     */
    std::shared_ptr<DialogueManager> DialogueManager::set_beep( std::string file ) {
        _beep_sound = file;
        return _instance;
    }

    /*
     * Set the text animation interval
     */
    std::shared_ptr<DialogueManager> DialogueManager::set_timer( float timer ) {
        if ( timer <= 0.0f ) {
            _has_timer = false;
        } else {
            _timer = timer;
            _has_timer = true;
        }
        return _instance;
    }

    /*
     * Initialize the dialogue
     */
    void DialogueManager::set_dialogue( std::string json_file ) {
        auto futils = FileUtils::getInstance();
        if ( !futils->isFileExist(json_file) ) {
            return false;
        }
        Document page = json_util::json_decode( json_file );
        _dialogueQueue.push(
            std::shared_ptr<d_lan::Page>(
                new d_lan::Page( page )
            );
        );
        return true;
    }

    /**/
    void DialogueManager::show_next( void ) {
        _dialogueQueue.front().reset();
        _dialogueQueue.pop();
    }

    /*
     * Listen for ENTER key press and show next dialogue message (with expression and name)
     */
    void DialogueManager::processInput( void ) {
        if ( _input->isPressed( EventKeyboard::KeyCode::KEY_ENTER ) ) {
            if ( !_dialogueQueue.size() > 0 ) {
                show_next();
            } else {
                _dialogue_view->setVisible( false );
            }
        }
    }

    void DialogueManager::update( float delta ) {
        if ( _dialogue_view->isVisible() ) {
            if ( _text_ended ) {
                processInput();
            } else {
                
            }
        }
    }
}
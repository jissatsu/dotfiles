#include "Input.h"

Input * Input::create( void )
{
    Input *input = new Input();
    if ( input ) {
        input->autorelease();
        return input;
    }
    CC_SAFE_DELETE( input );
    return nullptr;
}

void Input::push( EventKeyboard::KeyCode key )
{
    if( std::find(this->_pressed.begin(), this->_pressed.end(), key) == this->_pressed.end() ) {
        this->_pressed.push_back( key );
    }
}

void Input::pop( EventKeyboard::KeyCode key )
{
    this->_pressed.erase(
        std::remove(
            this->_pressed.begin(), this->_pressed.end(), key
        ),
        this->_pressed.end()
    );
}

// indicates some kind of movement (cursor, player, etc...)
bool Input::dir4( void )
{
    auto key = this->_pressed.back();
    if ( key == EventKeyboard::KeyCode::KEY_UP_ARROW   || key == EventKeyboard::KeyCode::KEY_DOWN_ARROW || 
         key == EventKeyboard::KeyCode::KEY_LEFT_ARROW || key == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ) {
        return true;
    }
    return false;
}

// usually the escape key is used to pause the game
bool Input::_pause( void )
{
    auto key = this->_pressed.back();
    if ( key == EventKeyboard::KeyCode::KEY_ESCAPE ) {
        return true;
    }
    return false;
}

EventKeyboard::KeyCode Input::last_key( void )
{
    if ( this->_pressed.empty() ) {
        return EventKeyboard::KeyCode::KEY_NONE;
    }
    return this->_pressed.back();
}

std::string Input::map( EventKeyboard::KeyCode key )
{
    return this->_keymap[key];
}
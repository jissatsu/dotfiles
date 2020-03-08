#include "CallbackManager.h"

CallbackManager * CallbackManager::create( void )
{
    CallbackManager *manager = new CallbackManager();
    if ( manager ) {
        manager->autorelease();
        return manager;
    }
    CC_SAFE_DELETE( manager );
    return nullptr;
}

void CallbackManager::push_call( std::string name, const std::function<void()> &func )
{
    this->call_names.push_back( name );
    this->_callbacks[name] = new std::function<void()>( func );
}

void CallbackManager::release_callback( std::string name )
{
    if ( this->_callbacks.find( name ) != this->_callbacks.end() ) {
        CC_SAFE_DELETE(
            this->_callbacks[name]
        );
    }
}

void CallbackManager::call( std::string name )
{
    if ( this->_callbacks.find( name ) != this->_callbacks.end() ) {
        (*this->_callbacks[name])();
    }
}

void CallbackManager::release_all( void )
{
    if ( this->call_names.size() > 0 ) {
        for ( auto name : this->call_names ) {
            CC_SAFE_DELETE(
                this->_callbacks[name]
            );
            // std::cout << "Deleted " << name << "()" << "\n";
        }
    }
}
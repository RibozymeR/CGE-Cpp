#include "UserInput.h"

using std::cerr;

UserInput::UserInput(void){}

UserInput::~UserInput(){
	cerr << "destroying UserInput..." << std::endl;
	cerr << "destroyed UserInput" << std::endl;
}

void UserInput::addMouseHandler(MOUSE_HANDLER handler){
	if(handler == NULL) return;
	mouse_handlers.push_back(handler);
}

void UserInput::removeMouseHandler(MOUSE_HANDLER handler){
	for(uint32_t i = 0; i < mouse_handlers.size(); ++i) if(mouse_handlers[i] == handler){
		mouse_handlers[i] = mouse_handlers.back();
		mouse_handlers.pop_back();
	}
}

void UserInput::addMouseMoveHandler(MOUSE_MOVE_HANDLER handler){
	if(handler == NULL) return;
	mousem_handlers.push_back(handler);
}

void UserInput::removeMouseMoveHandler(MOUSE_MOVE_HANDLER handler){
	for(uint32_t i = 0; i < mousem_handlers.size(); ++i) if(mousem_handlers[i] == handler){
		mousem_handlers[i] = mousem_handlers.back();
		mousem_handlers.pop_back();
	}
}

void UserInput::addMouseWheelHandler(MOUSE_WHEEL_HANDLER handler){
	if(handler == NULL) return;
	mousew_handlers.push_back(handler);
}

void UserInput::removeMouseWheelHandler(MOUSE_WHEEL_HANDLER handler){
	for(uint32_t i = 0; i < mousew_handlers.size(); ++i) if(mousew_handlers[i] == handler){
		mousew_handlers[i] = mousew_handlers.back();
		mousew_handlers.pop_back();
	}
}

void UserInput::addKeyHandler(KEY_HANDLER handler){
	if(handler == NULL) return;
	key_handlers.push_back(handler);
}

void UserInput::removeKeyHandler(KEY_HANDLER handler){
	for(uint32_t i = 0; i < key_handlers.size(); ++i) if(key_handlers[i] == handler){
		key_handlers[i] = key_handlers.back();
		key_handlers.pop_back();
	}
}

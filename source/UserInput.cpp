
/*  Implementation for UserInput.h
    Copyright (C) 2017  RibozymeR

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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

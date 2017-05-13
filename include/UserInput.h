#ifndef _USERINPUT_H_
#define _USERINPUT_H_

#include <vector>
#include "CGE.h"

using std::vector;

class UserInput{
	public:
		UserInput();
		virtual ~UserInput();
		
		void addMouseHandler(MOUSE_HANDLER handler);
		void removeMouseHandler(MOUSE_HANDLER handler);
		void addMouseMoveHandler(MOUSE_MOVE_HANDLER handler);
		void removeMouseMoveHandler(MOUSE_MOVE_HANDLER handler);
		void addMouseWheelHandler(MOUSE_WHEEL_HANDLER handler);
		void removeMouseWheelHandler(MOUSE_WHEEL_HANDLER handler);
		
		void addKeyHandler(KEY_HANDLER handler);
		void removeKeyHandler(KEY_HANDLER handler);
		
		virtual void pollEvents() = 0;
	
	protected:
		vector<MOUSE_HANDLER> mouse_handlers;
		vector<MOUSE_MOVE_HANDLER> mousem_handlers;
		vector<MOUSE_WHEEL_HANDLER> mousew_handlers;
		vector<KEY_HANDLER> key_handlers;
};

#endif
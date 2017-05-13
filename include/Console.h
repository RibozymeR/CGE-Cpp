#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "Context.h"
#include "UserInput.h"

class Console: public Context, public UserInput{
	public:
		Console(std::string title, int width, int height);
		Console(std::string title, int width, int height, char fg, char bg);
		virtual ~Console();
		
		std::string getTitle();
		void setTitle(std::string title);
	
		void setReduceDebug(bool reduced);
	
		void swapBuffers();
		void pollEvents();
	
	private:
		HANDLE hstdout, hstdin;
		COORD size;
		INPUT_RECORD *eventbuffer;
		bool reduce_debug;
};

#endif
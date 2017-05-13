#include "Console.h"

using std::string;
using std::endl;
using std::cerr;

Console::Console(string title, int width, int height) : Console(title, width, height, C_WHITE, C_BLACK){}

Console::Console(string title, int width, int height, char fg, char bg) : Context(width, height, fg, bg), UserInput(){
	hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hstdin = GetStdHandle(STD_INPUT_HANDLE);
	
	size = {(short)WIDTH, (short)HEIGHT};
	
	SetConsoleWindowInfo(hstdout, 1, &bounds);
	CONSOLE_CURSOR_INFO ci = {2, false};
	SetConsoleCursorInfo(hstdout, &ci);
	SetConsoleScreenBufferSize(hstdout, size);
	setTitle(title);
	
	eventbuffer = new INPUT_RECORD[32];
	
	reduce_debug = false;
}

Console::~Console(){
	cerr << "destroying Console..." << endl;
	FreeConsole();
	delete[] eventbuffer;
	cerr << "destroyed Console" << endl;
}
		
string Console::getTitle(){
	char *titlec = new char[64];
	GetConsoleTitle(titlec, 64);
	return titlec;
}

void Console::setTitle(string title){
	SetConsoleTitle(title.c_str());
}

void Console::setReduceDebug(bool reduced){
	reduce_debug = reduced;
}

void Console::swapBuffers(){
	WriteConsoleOutputA(hstdout, screen, size, {0, 0}, &bounds);
}

void Console::pollEvents(){
	DWORD events = 0;
	GetNumberOfConsoleInputEvents(hstdin, &events);
	if(events <= 0) return;
	
	DWORD eventsread = 0;
	ReadConsoleInput(hstdin, eventbuffer, events > 32 ? 32 : events, &eventsread);
	if(!reduce_debug) cerr << "number of events: " << events << ", read " << eventsread << endl;
	for(uint8_t i = 0; i < eventsread; ++i){
		if(eventbuffer[i].EventType == KEY_EVENT){
			if(!reduce_debug) cerr << "key event" << endl;
			KEY_EVENT_RECORD event = eventbuffer[i].Event.KeyEvent;
			for(register uint32_t i = 0; i < key_handlers.size(); ++i) (*(key_handlers[i]))((bool)event.bKeyDown, event.wVirtualKeyCode, event.uChar.AsciiChar, event.dwControlKeyState);
		}
		else if(eventbuffer[i].EventType == MOUSE_EVENT){
			MOUSE_EVENT_RECORD event = eventbuffer[i].Event.MouseEvent;
			register int type = event.dwEventFlags;
			if(type == 0 || type & DOUBLE_CLICK){
				if(!reduce_debug) cerr << "mouse event" << endl;
				COORD pos = event.dwMousePosition;
				for(register uint32_t i = 0; i < mouse_handlers.size(); ++i) (*(mouse_handlers[i]))(pos.X, pos.Y, event.dwButtonState, event.dwControlKeyState);
			}
			else if(type & MOUSE_MOVED){
				if(!reduce_debug) cerr << "mouse move event" << endl;
				COORD pos = event.dwMousePosition;
				for(register uint32_t i = 0; i < mousem_handlers.size(); ++i) (*(mousem_handlers[i]))(pos.X, pos.Y, event.dwButtonState, event.dwControlKeyState);
			}
			else if(type & MOUSE_WHEELED){
				if(!reduce_debug) cerr << "mouse wheel event" << endl;
				COORD pos = event.dwMousePosition;
				for(register uint32_t i = 0; i < mousew_handlers.size(); ++i) (*(mousew_handlers[i]))(pos.X, pos.Y, (signed short int)(event.dwButtonState >> 16), event.dwControlKeyState);
			}
		}
	}
}
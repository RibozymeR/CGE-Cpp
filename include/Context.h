#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <stack>
#include "CGE.h"
#include "Texture.h"

class Context{
	public:
		Context(int width, int height);
		Context(int width, int height, char fg, char bg);
		virtual ~Context();
		int getWidth();
		int getHeight();
		
		void setColor(char fg, char bg);
		void setFillMode(char mode);
		void setCustomFillMode(char c);
		void writeChar(char c, int x, int y);
		void writeString(std::string text, int x, int y);
		void writeStringf(std::string text, int x, int y);
		
		void pushState();
		void popState();
		
		void drawPoint(int x, int y);
		void drawLine(int x0, int y0, int x1, int y1);
		void drawRect(int x, int y, int width, int height);
		void clearRect(int x, int y, int width, int height);
		void fillRect(int x, int y, int width, int height);
		void drawEllipse(int x0, int y0, int x1, int y1);
		void flood(int x, int y);
		
		void drawTexture(Texture *texture, int x, int y);
		void drawTexture(Texture *texture, int tx, int ty, int width, int height, int x, int y);
		Texture *storeScreen(Texture *dst);
		Texture *storeScreen(Texture *dst, int x, int y, int width, int height);
		
		void enableStencil(bool enable);
		void setStencil(int x, int y, int width, int height);
		void clearStencil(int x, int y, int width, int height);
		
		void bindShader(FRAGMENT_SHADER *shader);
		void applyShader(FRAGMENT_SHADER *shader);
		
		virtual void swapBuffers() = 0;
		void clear();
	
	protected:
		CHAR_INFO *screen;
		SMALL_RECT bounds = {0, 0, 0, 0};
		int WIDTH, HEIGHT;
		
	private:
		int SCRSIZE;
		char color;
		char filler;
		std::stack<char> statestack;
		bool stencil_enabled;
		bool *stencil;
		FRAGMENT_SHADER *fragment_shader = NULL;
		
		void point(int pos);
		void floodPos(int x, int y, CHAR_INFO *ground);
		void fillStencilRect(int x, int y, int width, int height, bool state);
};

#endif
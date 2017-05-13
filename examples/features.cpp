#include <fstream>
#include "Console.h"

#define sumsgninc(a, b) (((a) - (b)) + ((a) < (b) ? -1 : 1))

using std::cerr;

Console *console = new Console("Test", 80, 25, C_RED | C_DARK, C_GREEN | C_BLUE);

bool running = true;
int dx = 0, state = 0, px, py;

void shaderf(int x, int y, char colors[], bool *stencil){
	if(*stencil) colors[0] &= ~C_RED & ~C_BLUE;
	colors[1] &= ~C_RED & ~C_BLUE;
	*stencil = true;
}
FRAGMENT_SHADER shader = {&shaderf};

void mousing(int x, int y, int button, int controls){
	if(button == LEFT_BUTTON) console->drawPoint(x + dx, y);
	else if(button == MID_BUTTON){
		console->pushState();
		console->setFillMode(MODE_HALF);
		console->flood(x, y);
		console->popState();
	}
	else if(button == RIGHT_BUTTON){
		if(state) console->fillRect(px, py, sumsgninc(x, px), sumsgninc(y, py));
		else{ px = x; py = y; }
		state = ~state;
	}
}

void wheeling(int x, int y, int direction, int controls){
	dx += direction > 0 ? 1 : -1;
}

void keying(bool keydown, int keycode, char character, int controls){
	if(keycode == VK_ESCAPE) running = false;
	else if(keycode == VK_C) console->clear();
	else if(keycode == VK_S){
		Texture texture;
		console->storeScreen(&texture);
		texture.save("texture.cge");
	}
	else if(keycode == VK_L){
		Texture texture;
		texture.load("texture.cge");
		console->drawTexture(&texture, 0, 0);
	}
	else if(keycode == VK_1){
		console->enableStencil(true);
		console->clearStencil(0, 0, 40, 25);
	}
	else if(keycode == VK_2){
		console->enableStencil(false);
	}
	else if(keycode == VK_5){
		console->bindShader(&shader);
	}
	else if(keycode == VK_6){
		console->bindShader(NULL);
	}
	else if(keycode == VK_Y){
		console->applyShader(&shader);
	}
}

int main(){
	std::ofstream err("err.txt");
	auto cerrbuf = cerr.rdbuf(err.rdbuf());
	
	cerr << console->getWidth() << " " << console->getHeight() << std::endl;
	
	console->setReduceDebug(true);
	console->addMouseHandler(mousing);
	console->addMouseMoveHandler(mousing);
	console->addMouseWheelHandler(wheeling);
	console->addKeyHandler(keying);
	console->writeString("Draw lines!", 0, 0);
	console->swapBuffers();
	while(running){
		console->pollEvents();
		console->swapBuffers();
	}
	delete console;
	cerr << "stop." << std::endl;
	
	cerr.rdbuf(cerrbuf);
	return 0;
}

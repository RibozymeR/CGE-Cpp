#include <fstream>
#include "Console.h"

using std::cerr;

Console *console = new Console("Game of Life", 80, 25, C_BLACK, C_WHITE);

bool running;
void keying(bool keydown, int keycode, char character, int controls){
	if(keycode == VK_ESCAPE) running = false;
}

uint8_t life[2000], nlife[2000];
void step(){
	for(int x = 0; x < 80; ++x){
		for(int y = 0; y < 25; ++y){
			int n = 0;
			for(int i = 0; i < 9; ++i) n += life[(x + i % 3 - 1) % 80 + (y + i / 3 - 1) % 25 * 80];
			//for(int i = -1; i < 2; ++i) for(int j = -1; j < 2; ++j) n += life[(x + i) + 80 * (y + j)];
			nlife[x + 80 * y] = (n == 3 || (life[x + 80 * y] && n == 4)) ? 1 : 0;
		}
	}
	console->clear();
	for(int x = 0; x < 80; ++x){
		for(int y = 0; y < 25; ++y){
			int l = nlife[x + 80 * y];
			life[x + 80 * y] = l;
			if(l) console->drawPoint(x, y);
		}
	}
}

int main(){
	std::ofstream err("err.txt");
	auto errbuf = cerr.rdbuf(err.rdbuf());
	cerr << console->getWidth() << " " << console->getHeight() << std::endl;
	
	console->setReduceDebug(true);
	console->addKeyHandler(keying);
	console->clear();
	for(int i = 0; i < 2000; ++i) life[i] = 0;
	for(int x = 0; x < 8; ++x){
		for(int y = 0; y < 3; ++y){
			if(y == 1 && (x == 1 || x == 6)) continue;
			life[(36 + x) + (11 + y) * 80] = 1;
			console->drawPoint(36 + x, 11 + y);
		}
	}
	console->swapBuffers();
	Sleep(1000);
	
	running = true;
	while(running){
		step();
		console->pollEvents();
		console->swapBuffers();
		Sleep(400);
	}
	delete console;
	cerr << "stop" << std::endl;
	
	cerr.rdbuf(errbuf);
	return 0;
}

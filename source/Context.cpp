
/*  Implementation for Context.h
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

#include "Context.h"

using std::endl;
using std::cerr;

Context::Context(int width, int height): Context(width, height, C_WHITE, C_BLACK){}
	
Context::Context(int width, int height, char fg, char bg){
	WIDTH = width;
	HEIGHT = height;
	SCRSIZE = WIDTH * HEIGHT;
	bounds.Right = WIDTH - 1;
	bounds.Bottom = HEIGHT - 1;
	
	setColor(fg, bg);
	setFillMode(MODE_FILL);
	screen = new CHAR_INFO[SCRSIZE];
	stencil = new bool[SCRSIZE];
	for(int i = 0; i < SCRSIZE; ++i) stencil[i] = true;
	clear();
}

Context::~Context(){
	cerr << "destroying Context..." << endl;
	delete[] screen;
	if(stencil != NULL) delete[] stencil;
	cerr << "Context destroyed" << endl;
}

int Context::getWidth(){
	return WIDTH;
}
int Context::getHeight(){
	return HEIGHT;
}

//-----properties-----

void Context::setColor(char fg, char bg){
	if(fg == C_KEEP) color = (bg ^ 8) << 4 | (color & 0xF);
	else if(bg == C_KEEP) color = (color & 0xF0) | (fg ^ 8);
	else color = (bg << 4 | fg) ^ 0x88;
}

void Context::setFillMode(char mode){
	switch(mode){
		case MODE_FILL:  filler = 0xDB; break;
		case MODE_HALF:	 filler = 0xB1; break;
		case MODE_EMPTY: filler = ' ';  break;
		case MODE_DUST:	 filler = 0xB0; break;
		case MODE_DENSE: filler = 0xB2; break;
		case MODE_KEEP:  filler = 0;    break;
		default: filler = ' '; break;
	}
}

void Context::setCustomFillMode(char c){
	filler = c;
}

//-----statestack-----

void Context::pushState(void){
	statestack.push(color);
	statestack.push(filler);
	statestack.push(stencil_enabled);
}

void Context::popState(void){
	stencil_enabled = statestack.top();
	statestack.pop();
	filler = statestack.top();
	statestack.pop();
	color = statestack.top();
	statestack.pop();
}

//-----drawing-----

void Context::writeChar(char c, int x, int y){
	int pos = x + y * WIDTH;
	screen[pos].Char.AsciiChar = c;
	screen[pos].Attributes = color;
}

void Context::writeString(std::string text, int x, int y){
	int pos = x + y * WIDTH;
	for(size_t i = 0; i < text.size(); ++i){
		register char c = text[i];
		if(c){
			screen[pos].Char.AsciiChar = c;
			screen[pos++].Attributes = color;
		}
	}
}

void Context::writeStringf(std::string text, int x, int y){
	int pos = x + y * WIDTH;
	register bool ctrlmode = false;
	for(size_t i = 0; i < text.size(); ++i){
		register char c = text[i];
		if(!c) continue; //\0 doesn't do anything
		if(ctrlmode){
			if(c == '\\'){
				screen[pos].Char.AsciiChar = '\\';
				screen[pos++].Attributes = color;
			}
			else if(c == 'n') pos = x + (++y) * WIDTH;
			else if(c == 'q') return;
			else if(c == 'c') setColor(text[++i], C_KEEP);
			ctrlmode = false;
		}
		else{
			if(c == '\\') ctrlmode = true;
			else{
				screen[pos].Char.AsciiChar = c;
				screen[pos++].Attributes = color;
			}
		}
	}
}

void Context::point(int pos){
	bool paint = true;
	if(pos < 0 || pos >= SCRSIZE) return;
	register int pcolor = color;
	if(fragment_shader != NULL){
		paint = stencil[pos];
		char cparts[] = {(char)((pcolor & 15) ^ 8), (char)((pcolor >> 4) ^ 8)};
		(*fragment_shader->fragment)(pos % WIDTH, pos / WIDTH, cparts, &paint);
		pcolor = (cparts[1] << 4 | cparts[0]) ^ 0x88;
	}
	if(stencil_enabled && !paint) return;
	if(filler) screen[pos].Char.AsciiChar = filler;
	screen[pos].Attributes = pcolor;
}

void Context::drawPoint(int x, int y){
	if(x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) return;
	point(x + y * WIDTH);
}

void Context::drawLine(int x0, int y0, int x1, int y1){
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;
	while(1){
		drawPoint(x0, y0);
		if(x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if(e2 > dy){
			err += dy;
			x0 += sx;
		}
		if(e2 < dx){
			err += dx;
			y0 += sy;
		}
	}
}

void Context::drawRect(int x, int y, int width, int height){
	if(width < 0) x -= (width = -width);
	if(height < 0) y -= (height = -height);
	for(int tx = x; tx < x + width; ++tx){
		drawPoint(tx, y);
		drawPoint(tx, y + height - 1);
	}
	for(int ty = y + 1; ty < y + height - 1; ++ty){
		drawPoint(x, ty);
		drawPoint(x + width - 1, ty);
	}
}

void Context::clearRect(int x, int y, int width, int height){
	char last = filler;
	filler = ' ';
	fillRect(x, y, width, height);
	filler = last;
}

void Context::fillRect(int x, int y, int width, int height){
	if(width < 0) x -= (width = -width);
	if(height < 0) y -= (height = -height);
	register int incr = WIDTH - width, c = 0;
	for(register int i = x + y * WIDTH; i < x + width + (y + height - 1) * WIDTH; ++i){
		point(i);
		if(++c == width){
			c = 0;
			i += incr;
		}
	}
}

void Context::drawEllipse(int x0, int y0, int x1, int y1){
	int xm = (x0 + x1) >> 1, ym = (y0 + y1) >> 1;
	int a = x1 - x0, b = y1 - y0;
	a = (a < 0 ? -a : a) >> 1;
	b = (b < 0 ? -b : b) >> 1;
	int dx = 0, dy = b;
	long a2 = a * a, b2 = b * b;
	long err = b2 - (2 * b - 1) * a2, e2;
	do{
		drawPoint(xm + dx, ym + dy);
		drawPoint(xm + dx, ym - dy);
		drawPoint(xm - dx, ym + dy);
		drawPoint(xm - dx, ym - dy);
		e2 = 2 * err;
		if(e2 < (2 * dx + 1) * b2){
			++dx;
			err += (2 * dx + 1) * b2;
		}
		if(e2 > -(2 * dy - 1) * a2){
			--dy;
			err -= (2 * dy - 1) * a2;
		}
	}while(dy >= 0);
	while(dx++ < a){
		drawPoint(xm + dx, ym);
		drawPoint(xm - dx, ym);
	}
}

#define equscrgnd(a) (screen[a].Char.AsciiChar == ground->Char.AsciiChar && screen[a].Attributes == ground->Attributes)
bool *searched;
void Context::floodPos(int x, int y, CHAR_INFO *ground){
	int pos = x + y * WIDTH;
	searched[pos] = true;
	if(y > 0 && !searched[pos - WIDTH] && equscrgnd(pos - WIDTH)) floodPos(x, y - 1, ground);
	if(y < HEIGHT - 1 && !searched[pos + WIDTH] && equscrgnd(pos + WIDTH)) floodPos(x, y + 1, ground);
	if(x > 0 && !searched[pos - 1] && equscrgnd(pos - 1)) floodPos(x - 1, y, ground);
	if(x < WIDTH - 1 && !searched[pos + 1] && equscrgnd(pos + 1)) floodPos(x + 1, y, ground);
	point(pos);
}
#undef equscrgnd

void Context::flood(int x, int y){
	cerr << "starting flood from " << x << ", " << y << endl;
	searched = new bool[SCRSIZE];
	for(int i = 0; i < SCRSIZE; ++i) searched[i] = false;
	floodPos(x, y, &screen[x + y * WIDTH]);
	delete[] searched;
}

//-----textures-----

void Context::drawTexture(Texture *texture, int x0, int y0){
	cerr << "starting to draw texture" << endl;
	for(int x = 0; x < texture->width; ++x){
		int scrx = x0 + x;
		if(scrx < 0 || scrx >= WIDTH) continue;
		for(int y = 0; y < texture->height; ++y){
			int scry = y0 + y;
			if(scry < 0 || scry >= HEIGHT) continue;
			register int pos = scrx + scry * WIDTH;
			register CHAR_INFO pxl = texture->pixels[x + y * texture->width];
			if(pxl.Char.AsciiChar == 0 || (stencil_enabled && !stencil[pos])) continue;
			screen[pos] = pxl;
		}
	}
	cerr << "drew texture" << endl;
}

void Context::drawTexture(Texture *texture, int tx, int ty, int width, int height, int x0, int y0){
	if(width > texture->width || height > texture->height){
		cerr << "part is too big" << endl;
		return;
	}
	cerr << "starting to draw texture" << endl;
	for(int x = tx; x < width; ++x){
		int scrx = x0 + x;
		if(scrx < 0 || scrx >= WIDTH) continue;
		for(int y = ty; y < height; ++y){
			int scry = y0 + y;
			if(scry < 0 || scry >= HEIGHT) continue;
			register int pos = scrx + scry * WIDTH;
			register CHAR_INFO pxl = texture->pixels[x + y * texture->width];
			if(pxl.Char.AsciiChar == 0 || (stencil_enabled && !stencil[pos])) continue;
			screen[pos] = pxl;
		}
	}
	cerr << "drew texture" << endl;
}

Texture *Context::storeScreen(Texture *dst){
	cerr << "storing texture from 0, 0 to " << WIDTH - 1 << ", " << HEIGHT - 1 << endl;
	dst->width = WIDTH;
	dst->height = HEIGHT;
	delete[] dst->pixels;
	dst->pixels = new CHAR_INFO[WIDTH * HEIGHT];
	for(register int tx = 0; tx < WIDTH; ++tx){
		for(register int ty = 0; ty < HEIGHT; ++ty){
			register int pos = tx + ty * WIDTH;
			if(stencil_enabled && !stencil[pos]) continue;
			dst->pixels[pos] = screen[pos];
		}
	}
	cerr << "stored texture" << endl;
	return dst;
}

Texture *Context::storeScreen(Texture *dst, int x, int y, int width, int height){
	cerr << "storing texture from " << x << ", " << y << " to " << x + width - 1 << ", " << y + height - 1 << endl;
	dst->width = width;
	dst->height = height;
	delete[] dst->pixels;
	dst->pixels = new CHAR_INFO[width * height];
	for(register int tx = 0; tx < width; ++tx){
		register int scrx = x + tx;
		if(scrx < 0 || scrx >= WIDTH) continue;
		for(register int ty = 0; ty < height; ++ty){
			register int scry = y + ty;
			register int pos = scrx + scry * WIDTH;
			if(scry < 0 || scry >= HEIGHT || (stencil_enabled && !stencil[pos])) continue;
			dst->pixels[tx + ty * width] = screen[pos];
		}
	}
	cerr << "stored texture" << endl;
	return dst;
}

//-----stencil-----

void Context::enableStencil(bool enable){
	stencil_enabled = enable;
}

void Context::fillStencilRect(int x, int y, int width, int height, bool state){
	if(width < 0) x -= (width = -width);
	if(height < 0) y -= (height = -height);
	register int incr = WIDTH - width, c = 0;
	for(register int i = x + y * WIDTH; i < x + width + (y + height - 1) * WIDTH; ++i){
		stencil[i] = state;
		if(++c == width){
			c = 0;
			i += incr;
		}
	}
}

void Context::setStencil(int x, int y, int width, int height){
	fillStencilRect(x, y, width, height, true);
}

void Context::clearStencil(int x, int y, int width, int height){
	fillStencilRect(x, y, width, height, false);
}

//-----shaders-----

void Context::bindShader(FRAGMENT_SHADER *shader){
	fragment_shader = shader;
}

void Context::applyShader(FRAGMENT_SHADER *shader){
	if(shader == NULL) return;
	register int pos = 0;
	char cparts[2];
	bool paint;
	for(int y = 0; y < HEIGHT; ++y){
		for(int x = 0; x < WIDTH; ++x){
			register int color = screen[pos].Attributes ^ 0x88;
			cparts[0] = color & 15;
			cparts[1] = color >> 4;
			paint = stencil[pos];
			(*shader->fragment)(x, y, cparts, &paint);
			if(paint) screen[pos].Attributes = (cparts[1] << 4 | cparts[0]) ^ 0x88;
			++pos;
		}
	}
}

//-----clear-----

void Context::clear(void){
	for(int i = 0; i < SCRSIZE; ++i){
		screen[i].Char.AsciiChar = ' ';
		screen[i].Attributes = color;
	}
}

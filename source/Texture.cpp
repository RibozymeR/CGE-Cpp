#include <fstream>
#include <ios>
#include "Texture.h"

using std::endl;
using std::cerr;
using std::string;
using std::istream;
using std::ostream;

Texture::Texture(){
	width = 0;
	height = 0;
}

Texture::Texture(int w, int h){
	width = w;
	height = h;
	pixels = new CHAR_INFO[w * h];
}

Texture::Texture(char pxls[], int w, int h) : Texture(w, h){
	for(int pos = 0; pos < width * height; ++pos){
		pixels[pos].Char.AsciiChar = 0xDB;
		pixels[pos].Attributes = 0xF0 | pxls[pos];
	}
}

Texture::Texture(string file){
	load(file);
}

Texture::Texture(istream *src){
	load(src);
}

Texture::~Texture(){
	delete[] pixels;
}

int Texture::getWidth(){
	return width;
}

int Texture::getHeight(){
	return height;
}

void Texture::load(string srcfile){
	std::ifstream file;
	file.open(srcfile, std::ios::binary);
	load(&file);
	file.close();
}

void Texture::load(std::istream *src){
	cerr << "loading texture" << endl;
	if(src == NULL) return;
	char *head = new char[6];
	src->read(head, 6); //"CGE " + version number "01"
	cerr << "header: " << head << endl;
	delete[] head;
	src->read((char *)&width, 4);
	src->read((char *)&height, 4);
	cerr << "read size " << width << ", " << height << endl;
	register int size = width * height;
	delete[] pixels;
	pixels = new CHAR_INFO[size];
	cerr << "loading pixels" << endl;
	char *buffer = new char[2 * size];
	src->read(buffer, 2 * size);
	for(int i = 0; i < size; ++i){
		pixels[i].Char.AsciiChar = buffer[i << 1];
		pixels[i].Attributes = buffer[i << 1 | 1];
	}
	delete[] buffer;
	cerr << "loaded texture" << endl;
}

void Texture::save(string dstfile){
	std::ofstream file;
	file.open(dstfile, std::ios::binary);
	save(&file);
	file.close();
}

void Texture::save(std::ostream *dst){
	cerr << "saving texture with size " << width << ", " << height << endl;
	if(dst == NULL) return;
	dst->write("CGE 01", 6);
	dst->write((char *)&width, 4);
	dst->write((char *)&height, 4);
	register int size = width * height;
	char *buffer = new char[2 * size];
	for(int i = 0; i < size; ++i){
		CHAR_INFO *pxl = &pixels[i];
		buffer[i << 1] = pxl->Char.AsciiChar;
		buffer[i << 1 | 1] = pxl->Attributes;
	}
	dst->write(buffer, 2 * size);
	delete[] buffer;
	cerr << "saved texture" << endl;
	
	/*int *equality = new int[size];
	equality[size - 1] = 0;
	CHAR_INFO *now = NULL, *last = &pixels[size - 1];
	for(int ix = size - 2; i >= 0; --i){
		now = &pixels[ix];
		if(now->Attributes == last->Attributes && now->Char.AsciiChar == last->Char.Attributes) equality[ix] = equality[ix + 1] + 1;
		last = now;
	}
	bool state = false;
	for(int ix = 0; ix < size; ++ix){
		if(state){
			if(equality[ix] == 0) state = false;
			equality[ix] = 1;
		}
		else{
			if(equality[ix] >= 2){
				++equality[ix];
				state = true;
			}
			else equality[ix] = 0;
		}
	}
	char *buffer = new char[2 * size];
	int len = 0, ix = 0, n;
	while(ix < size){
		n = equality[ix];
		if(n > 0){
			buffer[len++] = 1;
			buffer[len++] = (n >> 8) & 0xFF;
			buffer[len++] = n & 0xFF;
			buffer[len++] = 
		}
	}*/
}

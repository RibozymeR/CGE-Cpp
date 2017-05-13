#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "CGE.h"

class Texture{
	friend class Context;
	
	public:
		Texture();
		Texture(int w, int h);
		Texture(char pixels[], int w, int h);
		Texture(std::string file);
		Texture(std::istream *src);
		~Texture();
		
		int getWidth();
		int getHeight();
		
		void load(std::string srcfile);
		void load(std::istream *src);
		void save(std::string dstfile);
		void save(std::ostream *dst);
	
	private:
		int32_t width, height;
		CHAR_INFO *pixels = NULL;
};

#endif
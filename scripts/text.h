#pragma once
#ifdef _TEXT
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>
#include "OpenGL.h"
#include <map>
#include "globalVars.h"
#include "constants.h"
#include "IntializeShader.h"
#include "Shapes.h"
#endif

#if defined(_TEXT) || defined(_FLOW_CONTROL)
namespace Text {
	void start();
	void end();
}
#endif


#include "typeDefs.h"
#include "Vector.h"
#include <string>

#define TEXT_LINE_LEFT_RENDER 0
#define TEXT_LINE_CENTER_RENDER 1
#define TEXT_CENTER_RENDER 2


struct cChar {
	uint texTarget;
	Vec2 bearing;
	Vec2 size;
	float advance;

	cChar() : texTarget(0), bearing(0), size(0), advance(0) {}
	cChar(uint tex, Vec2 b, Vec2 s, float a) : texTarget(tex), bearing(b), size(s), advance(a) {}
};


bool loadFont(std::string font, std::string name);
cChar getChar(std::string name, char character);
void createTextTexture(uint& texture, float fontSize, float lineSize, Vec2 size, float yStart, float margin, uint mode, std::string font, std::string text);
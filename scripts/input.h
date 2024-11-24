#pragma once

#if defined(_INPUT)
#include "OpenGL.h"

#endif

#include "typeDefs.h"
#include <string>

struct buttonState {
	bool pressed = false;
	bool held = false;
	bool released = false;

	buttonState() : pressed(false), held(false), released(false) {}
};

namespace giveKeyAction {
	void keyPressed(int key);
	void keyReleased(int key);
	void latchSet();
	void scrolledUp();
	void scrolledDown();
	void setNumlock(bool val);
	void setShift(bool val);
	void setIgnore(bool val);
}

namespace keyAction {
	bool keyPressed(int key);
	bool keyHeld(int key);
	bool keyReleased(int key);
	bool scrolling();
	bool scrollUp();
	bool scrollDown();
	bool getNumlock();
	bool getShift();
	void buildText(std::string& text);
}
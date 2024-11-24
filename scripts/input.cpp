#define _INPUT
#include "input.h"

constexpr uint keyCount = 349;

static bool numlock = false;
static bool shift = false;

buttonState allButtons[keyCount];
bool scrollingUp = false;
bool scrollingDown = false;

bool ignoreOutside = false;

namespace giveKeyAction {
	void keyPressed(int key) {
		allButtons[key].pressed = true;
		allButtons[key].held = true;
		allButtons[key].released = false;
	}
	void keyReleased(int key) {
		allButtons[key].pressed = false;
		allButtons[key].held = false;
		allButtons[key].released = true;
	}
	void latchSet() {
		for (int i = 0; i < keyCount; i++) {
			allButtons[i].pressed = false;
			allButtons[i].released = false;
		}
		scrollingUp = false;
		scrollingDown = false;
	}
	void scrolledUp() {
		scrollingUp = true;
		scrollingDown = false;
	}
	void scrolledDown() {
		scrollingUp = false;
		scrollingDown = true;
	}
	void setNumlock(bool val) {
		numlock = val;
	}
	void setShift(bool val) {
		shift = val;
	}
	void setIgnore(bool val) {
		ignoreOutside = val;
	}
}

namespace keyAction {
	bool keyPressed(int key) {
		return ignoreOutside ? false : allButtons[key].pressed;
	}
	bool keyHeld(int key) {
		return ignoreOutside ? false : allButtons[key].held;
	}
	bool keyReleased(int key) {
		return ignoreOutside ? false : allButtons[key].released;
	}
	bool scrolling() {
		return ignoreOutside ? false : scrollingUp || scrollingDown;
	}
	bool scrollUp() {
		return ignoreOutside ? false : scrollingUp;
	}
	bool scrollDown() {
		return ignoreOutside ? false : scrollingDown;
	}
	bool getNumlock() {
		return ignoreOutside ? false : numlock;
	}
	bool getShift() {
		return ignoreOutside ? false : shift;
	}

	void buildText(std::string& text) {
		if (allButtons[GLFW_KEY_BACKSPACE].pressed && text.length())		text.pop_back();
		if (allButtons[GLFW_KEY_SPACE].pressed)										text += ' ';
		if (allButtons[GLFW_KEY_ENTER].pressed)										text += '\n';
		if (allButtons[GLFW_KEY_TAB].pressed)										text += '\t';
		if (allButtons[GLFW_KEY_A].pressed)											if (shift) text += 'A'; else text += 'a';
		if (allButtons[GLFW_KEY_B].pressed)											if (shift) text += 'B'; else text += 'b';
		if (allButtons[GLFW_KEY_C].pressed)											if (shift) text += 'C'; else text += 'c';
		if (allButtons[GLFW_KEY_D].pressed)											if (shift) text += 'D'; else text += 'd';
		if (allButtons[GLFW_KEY_E].pressed)											if (shift) text += 'E'; else text += 'e';
		if (allButtons[GLFW_KEY_F].pressed)											if (shift) text += 'F'; else text += 'f';
		if (allButtons[GLFW_KEY_G].pressed)											if (shift) text += 'G'; else text += 'g';
		if (allButtons[GLFW_KEY_H].pressed)											if (shift) text += 'H'; else text += 'h';
		if (allButtons[GLFW_KEY_I].pressed)											if (shift) text += 'I'; else text += 'i';
		if (allButtons[GLFW_KEY_J].pressed)											if (shift) text += 'J'; else text += 'j';
		if (allButtons[GLFW_KEY_K].pressed)											if (shift) text += 'K'; else text += 'k';
		if (allButtons[GLFW_KEY_L].pressed)											if (shift) text += 'L'; else text += 'l';
		if (allButtons[GLFW_KEY_M].pressed)											if (shift) text += 'M'; else text += 'm';
		if (allButtons[GLFW_KEY_N].pressed)											if (shift) text += 'N'; else text += 'n';
		if (allButtons[GLFW_KEY_O].pressed)											if (shift) text += 'O'; else text += 'o';
		if (allButtons[GLFW_KEY_P].pressed)											if (shift) text += 'P'; else text += 'p';
		if (allButtons[GLFW_KEY_Q].pressed)											if (shift) text += 'Q'; else text += 'q';
		if (allButtons[GLFW_KEY_R].pressed)											if (shift) text += 'R'; else text += 'r';
		if (allButtons[GLFW_KEY_S].pressed)											if (shift) text += 'S'; else text += 's';
		if (allButtons[GLFW_KEY_T].pressed)											if (shift) text += 'T'; else text += 't';
		if (allButtons[GLFW_KEY_U].pressed)											if (shift) text += 'U'; else text += 'u';
		if (allButtons[GLFW_KEY_V].pressed)											if (shift) text += 'V'; else text += 'v';
		if (allButtons[GLFW_KEY_W].pressed)											if (shift) text += 'W'; else text += 'w';
		if (allButtons[GLFW_KEY_X].pressed)											if (shift) text += 'X'; else text += 'x';
		if (allButtons[GLFW_KEY_Y].pressed)											if (shift) text += 'Y'; else text += 'y';
		if (allButtons[GLFW_KEY_Z].pressed)											if (shift) text += 'Z'; else text += 'z';
		if (allButtons[GLFW_KEY_APOSTROPHE].pressed)								if (shift) text += '\"'; else text += '\'';
		if (allButtons[GLFW_KEY_COMMA].pressed)										if (shift) text += '<'; else text += ',';
		if (allButtons[GLFW_KEY_MINUS].pressed)										if (shift) text += '_'; else text += '-';
		if (allButtons[GLFW_KEY_PERIOD].pressed)									if (shift) text += '>'; else text += '.';
		if (allButtons[GLFW_KEY_SLASH].pressed)										if (shift) text += '?'; else text += '/';
		if (allButtons[GLFW_KEY_SEMICOLON].pressed)									if (shift) text += ':'; else text += ';';
		if (allButtons[GLFW_KEY_EQUAL].pressed)										if (shift) text += '+'; else text += '=';
		if (allButtons[GLFW_KEY_LEFT_BRACKET].pressed)								if (shift) text += '{'; else text += '[';
		if (allButtons[GLFW_KEY_BACKSLASH].pressed)									if (shift) text += '|'; else text += '\\';
		if (allButtons[GLFW_KEY_RIGHT_BRACKET].pressed)								if (shift) text += '}'; else text += ']';
		if (allButtons[GLFW_KEY_GRAVE_ACCENT].pressed)								if (shift) text += '~'; else text += '`';
		if (allButtons[GLFW_KEY_1].pressed)											if (shift) text += '!'; else text += '1';
		if (allButtons[GLFW_KEY_2].pressed)											if (shift) text += '@'; else text += '2';
		if (allButtons[GLFW_KEY_3].pressed)											if (shift) text += '#'; else text += '3';
		if (allButtons[GLFW_KEY_4].pressed)											if (shift) text += '$'; else text += '4';
		if (allButtons[GLFW_KEY_5].pressed)											if (shift) text += '%'; else text += '5';
		if (allButtons[GLFW_KEY_6].pressed)											if (shift) text += '^'; else text += '6';
		if (allButtons[GLFW_KEY_7].pressed)											if (shift) text += '&'; else text += '7';
		if (allButtons[GLFW_KEY_8].pressed)											if (shift) text += '*'; else text += '8';
		if (allButtons[GLFW_KEY_9].pressed)											if (shift) text += '('; else text += '9';
		if (allButtons[GLFW_KEY_0].pressed)											if (shift) text += ')'; else text += '0';
	}
}
#define _FLOW_CONTROL
#include "flowControl.h"
void progStart();
void progMain();
void progEnd();

//App globals

Vec4 _bgColor = Vec4(0);
iVec2 _windowPos = iVec2(0);
iVec2 _realMousePos = iVec2(0);

bool _windowScaled = false;
bool _lockMouse = false;

float _mousePosX = 0;
float _mousePosY = 0;

extern Camera MainCamera;


//Local globals


void start() {
	for (uint i = 0; i < _MAX_OBJECTS; i++) {
		globalObjects[i] = nullptr;
	}

    Objects::start();
    Text::start();
    ProgUI::start();
    Sound::start();


	addShader("noTextureShader", "./shaders/NTVertexShader.vert", "./shaders/NTFragmentShader.frag");
	addShader("textureShader", "./shaders/TVertexShader.vert", "./shaders/TFragmentShader.frag");

    stbi_set_flip_vertically_on_load(true);

	progStart();
}

void update() {
    Sound::update();

    progMain();
    ProgUI::update();

	glClearColor(_bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    drawAllObjs();
	_windowScaled = false;

	giveKeyAction::latchSet();
}

void end() {
	progEnd();
	deleteAll();

	Text::end();
	Objects::end();
	Shaders::end();
    Sound::end();
}


//-------------------Functions

void setBgColor(Vec4 color) {
	_bgColor = color;
}
void setBgColor(float r, float g, float b, float a) {
	_bgColor = Vec4(r, g, b, a);
}


//-------------------CALLBACKS

void windowScaleCallback(GLFWwindow* window, int width, int height) {
	_Width = width;
	_Height = height;
	_screenRatio = (float)_Width / _Height;
	glViewport(0, 0, width, height);
	_windowScaled = true;
    MainCamera.setOrthographicView(_screenRatio, 1, 1);
    MainCamera.setPerspectiveView(MainCamera.FOV, _screenRatio, MainCamera.nearClip, MainCamera.farClip);
}
void windowMoveCallback(GLFWwindow* window, int xpos, int ypos) {
	_windowPos.x = xpos;
	_windowPos.y = ypos;
}
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	if (_lockMouse) {
		SetCursorPos(_windowPos.x + _realMousePos.x, _windowPos.y + _realMousePos.y);
		return;
	}

	_realMousePos = { xpos, ypos };
	_mousePosX = xpos / _Height * 2 - _screenRatio;
	_mousePosY = (_Height - ypos) / _Height * 2 - 1;
}
void keyPressCallback(GLFWwindow* window, int button, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		giveKeyAction::keyPressed(button);
	else if (action == GLFW_RELEASE)
		giveKeyAction::keyReleased(button);
	giveKeyAction::setNumlock(mods & GLFW_MOD_NUM_LOCK);
	giveKeyAction::setShift(mods & GLFW_MOD_SHIFT);
}
void mouseScrollCallback(GLFWwindow* window, double xoff, double yoff) {
	if (yoff > 0)
		giveKeyAction::scrolledUp();
	else if (yoff < 0)
		giveKeyAction::scrolledDown();
}
void mouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS)
		giveKeyAction::keyPressed(button);
	else if (action == GLFW_RELEASE)
		giveKeyAction::keyReleased(button);
}
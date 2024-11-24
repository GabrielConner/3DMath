#pragma once
#include "TypeDefs.h"
#include "OpenGL.h"
#include "Objects.h"

void setBgColor(Vec4 color);
void setBgColor(float r, float g, float b, float a);

#if defined(_FLOW_CONTROL) || defined(_PROGRAM_CONTROL)
void start();
void update();
void end();

void windowMoveCallback(GLFWwindow* window, int xpos, int ypos);
void windowScaleCallback(GLFWwindow* window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void keyPressCallback(GLFWwindow* window, int button, int scancode, int action, int mods);
void mouseScrollCallback(GLFWwindow* window, double xoff, double yoff);
#endif


#if defined(_FLOW_CONTROL)
#include "Shapes.h"
#include "GlobalVars.h"
#include <iostream>
#include "Input.h"
#include "Constants.h"
#include "MathConstants.h"
#include "Text.h"
#include <windows.h>
#include "ProgUI.h"
#include "stb_image.h"
#include "soundLoading.h"
#endif
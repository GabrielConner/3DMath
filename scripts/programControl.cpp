#define _PROGRAM_CONTROL
#include "ProgramControl.h"
#include "Windows.h"
#include <chrono>

uint _Width = 1280;
uint _Height = 720;
float _screenRatio = (float)_Width / _Height;
double _deltaTime = 0;
bool _closeApp = false;
bool _hideMouse = false;
bool _vsync = false;

int main() {
	if (!glfwInit()) {
		std::cout << "Failed to load glfw\n";
		return 0;
	}

	GLFWwindow* mainWindow = glfwCreateWindow(_Width, _Height, "My window", nullptr, nullptr);
	if (!mainWindow) {
		std::cout << "Failed to load window\n";
		return 0;
	}
	glfwMakeContextCurrent(mainWindow);
	glfwSetWindowSizeCallback(mainWindow, windowScaleCallback);
	glfwSetCursorPosCallback(mainWindow, mouseMoveCallback);
	glfwSetMouseButtonCallback(mainWindow, mouseClickCallback);
	glfwSetKeyCallback(mainWindow, keyPressCallback);
	glfwSetScrollCallback(mainWindow, mouseScrollCallback);
	glfwSetWindowPosCallback(mainWindow, windowMoveCallback);


	glfwSetInputMode(mainWindow, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	if (!gladLoadGL()) {
		std::cout << "Failed to load GL\n";
		return 0;
	}

	start();

	glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

	size_t delay = 0;
	while (!glfwWindowShouldClose(mainWindow)) 
	{
		std::chrono::time_point i = std::chrono::high_resolution_clock().now();
		glfwPollEvents();
		update();
		glfwSwapBuffers(mainWindow);

        if (_hideMouse) {
            glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
		else
			glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        if (_closeApp)
            glfwSetWindowShouldClose(mainWindow, 1);
        if (_vsync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

		delay = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock().now() - i).count();
		_deltaTime = (double)delay / 1000000;
    }

	end();

	return 1;
}



//Windows only
static int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow) {
	main();
}

//Class operators
std::ostream& operator <<(std::ostream& stream, const Vec2& val) {
    stream << val.x << ", " << val.y;
    return stream;
}

std::ostream& operator <<(std::ostream& stream, const Vec3& val) {
    stream << val.x << ", " << val.y << ", " << val.z;
    return stream;
}
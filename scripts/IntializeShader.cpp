#include "IntializeShader.h"


std::map<std::string, uint> globalShaders;

/// <summary>
/// Add a new shader to global shaders
/// </summary>
/// <param name="name">: New shader name</param>
/// <param name="vertLoc">: File location of vertex shader</param>
/// <param name="fragLoc">: File location of fragment shader</param>
/// <returns>True/False on if a new shader was added to global shaders</returns>
bool addShader(std::string name, std::string vertLoc, std::string fragLoc) {
	uint tempShader = Shader::createShader(vertLoc, fragLoc);
	auto sucess = globalShaders.insert({ name, tempShader });
	return sucess.second;
}

/// <summary>
/// Get a shader by name
/// </summary>
/// <param name="name">: Shader name</param>
/// <returns>Shader uint value, or 0 if not present</returns>
uint getShader(std::string name) {
	try {
		return globalShaders.at(name);
	}
	catch (const std::out_of_range& err) {
		std::cout << err.what() << "\n";
		std::cout << ":( that is not a present shader\n";
	}
	return 0;
}

namespace Shaders {
	void end() {
		for (const std::pair<std::string, uint> iter : globalShaders) {
			glDeleteProgram(iter.second);
		}
		globalShaders.clear();
	}
}
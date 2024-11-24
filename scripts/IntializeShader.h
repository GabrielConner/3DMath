#pragma once
#include "OpenGL.h"
#include "typeDefs.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Vector.h"
#include <map>

struct Shader {

	void active() {
		glUseProgram(ID);
	}
	void use(unsigned int shader) {
		ID = shader;
		glUseProgram(ID);
	}
	void remove() {
		glDeleteProgram(ID);
		ID = 0;
	}

	void stop() {
		glUseProgram(0);
	}
	void setUint(std::string name, uint value) {
		glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setInt(std::string name, int value) {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(std::string name, float value) {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec2(std::string name, float x, float y) {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	void setVecv2(std::string name, Vec2 vec) {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y);
	}
	void setVec3(std::string name, float x, float y, float z) {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setVec3(std::string name, Vec3 vec) {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
	}
	void setVec4(std::string name, float x, float y, float z, float w) {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	void setVec4(std::string name, Vec4 vec) {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
	}
	void setMat4(std::string name, glm::mat4& Mat) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, glm::value_ptr(Mat));
	}
    void setMat4(std::string name, const float*& Mat) {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, Mat);
    }

	static uint createShader(std::string vertexLoc, std::string fragmentLoc) {

		uint shader = 0;

		std::fstream vertexFile;
		std::fstream fragmentFile;

		const char* vShaderCode;
		const char* fShaderCode;

		std::string vString;
		std::string fString;

		try {
			vertexFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
			fragmentFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

			vertexFile.open(vertexLoc, std::ios::binary | std::ios::in);
			fragmentFile.open(fragmentLoc, std::ios::binary | std::ios::in);

			std::stringstream vStringStream;
			std::stringstream fStringStream;

			vStringStream << vertexFile.rdbuf();
			vString = vStringStream.str();
			vShaderCode = vString.c_str();

			fStringStream << fragmentFile.rdbuf();
			fString = fStringStream.str();
			fShaderCode = fString.c_str();
		}
		catch (std::ios_base::failure& fail) {
			std::cout << "Failed to open shader files\n Error message : \n" << fail.what();
		}

		uint vertexShader, fragmentShader;
		int success;
		char infoLog[512];

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			std::cout << "Failed to compile vertex shader\n INFO : \n" << infoLog;
		}


		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			std::cout << "Failed to compile fragment shader\n INFO : \n" << infoLog;
		}

		shader = glCreateProgram();
		glAttachShader(shader, vertexShader);
		glAttachShader(shader, fragmentShader);
		glLinkProgram(shader);

		glGetShaderiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cout << "Failed to link program\n INFO : \n" << infoLog;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shader;
	}

	private :
		uint ID;
};

bool addShader(std::string name, std::string vertLoc, std::string fragLoc);
uint getShader(std::string name);

namespace Shaders {
	void end();
}
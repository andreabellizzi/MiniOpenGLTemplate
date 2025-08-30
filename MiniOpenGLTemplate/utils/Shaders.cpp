// remove warning of unsecure version of fopen
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <errno.h>
#include <stdexcept>
#include <string>
#include "shaders.hpp"


void Shader::use(void) const {
	glUseProgram(programId);
}

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile) {
	ShaderProgram_t shaders[2] = {
		{vertexShaderFile, GL_VERTEX_SHADER},
		{fragmentShaderFile, GL_FRAGMENT_SHADER}
	};
	programId = createShader(shaders, 2);
	if (programId == 0) {
		throw std::invalid_argument("Error on creating shader program");
	}
}

Shader::Shader(ShaderProgram_t* shaders, unsigned int size) {
	programId = createShader(shaders, size);
	if (programId == 0) {
		throw std::invalid_argument("Error on creating shader program");
	}
}

void Shader::setUniform(const char* uniformName, float f1) const {
	unsigned int uniformLocation = glGetUniformLocation(programId, uniformName);
	if (uniformLocation == -1) {
		throw std::invalid_argument(std::string("Uniform name ")+uniformName+std::string(" location not found"));
	}
	glUniform1f(uniformLocation, f1);
}

void Shader::setUniform(const char* uniformName, int i1) const {
	unsigned int uniformLocation = glGetUniformLocation(programId, uniformName);
	if (uniformLocation == -1) {
		throw std::invalid_argument(std::string("Uniform name ") + uniformName + std::string(" location not found"));
	}
	glUniform1i(uniformLocation, i1);
}

void Shader::setUniform4fv(const char* uniformName, float* fptr) const {
	unsigned int uniformLocation = glGetUniformLocation(programId, uniformName);
	if (uniformLocation == -1) {
		throw std::invalid_argument(std::string("Uniform name ") + uniformName + std::string(" location not found"));
	}
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, fptr);
}


Shader::~Shader() {
	glDeleteProgram(programId);
}



bool checkShader(unsigned int id, unsigned int pname) {
	int success = 0;
	char infoLog[512];
	if (pname == GL_COMPILE_STATUS) {
		glGetShaderiv(id, pname, &success);
		if (success == GL_FALSE) {
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			return false;
		}
	}
	else if (pname == GL_LINK_STATUS) {
		glGetProgramiv(id, pname, &success);
		if (success == GL_FALSE) {
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
			return false;
		}
	}
	return true;
}

unsigned int createShader(ShaderProgram_t* shaders, unsigned int size) {
	FILE* shaderFile;
	unsigned int fileLength;
	unsigned int shaderIdx = 0;
	unsigned int* shadersId;
	unsigned int shaderProgram = 0;
	char* fullFile;
	shadersId = new unsigned int[size];

	shaderProgram = glCreateProgram();
	if (shaderProgram == 0) {
		goto CLEANUP;
	}


	for (shaderIdx = 0; shaderIdx < size; shaderIdx++) {
		shadersId[shaderIdx] = 0;
		shaderFile = fopen(shaders[shaderIdx].file, "rb");

		if (shaderFile) {
			fseek(shaderFile, 0, SEEK_END);
			fileLength = ftell(shaderFile);
			fseek(shaderFile, 0, SEEK_SET);
			fullFile = new char[fileLength + 1];
			fread(fullFile, 1, (size_t)fileLength, shaderFile);
			fullFile[fileLength] = '\0';
			fclose(shaderFile);
			shadersId[shaderIdx] = glCreateShader(shaders[shaderIdx].shaderType);
			if (shadersId[shaderIdx] == 0) {
				break;
			}
			glShaderSource(shadersId[shaderIdx], 1, &fullFile, NULL);
			glCompileShader(shadersId[shaderIdx]);
			delete[] fullFile;
			if (!checkShader(shadersId[shaderIdx], GL_COMPILE_STATUS)) {
				break;
			}
			glAttachShader(shaderProgram, shadersId[shaderIdx]);
		}
		else {
			fprintf(stderr, "%s:%d:%s: can't open %s: %s\n", __FILE__, __LINE__, __func__, shaders[shaderIdx].file, strerror(errno));
			break;
		}
	}

	if (shaderIdx == size) { //we've compiled all the shaders
		glLinkProgram(shaderProgram);
		if (!checkShader(shaderProgram, GL_LINK_STATUS)) {
			glDeleteProgram(shaderProgram);
			shaderProgram = 0;
		}
	}
	else {
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}

CLEANUP:
	// cleanup
	for (unsigned int i = 0; i < shaderIdx; i++) {
		if (shadersId[i] != 0) {
			glDeleteShader(shadersId[i]);
		}
	}
	delete[] shadersId;
	return shaderProgram;
}

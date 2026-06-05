#include "Rendering/Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::~Shader() {
	Destroy();
}

bool Shader::LoadFromFiles(const string& vertexPath, const string& fragmentPath) {
	Destroy();

	const string vertexSource = ReadFile(vertexPath);
	const string fragmentSource = ReadFile(fragmentPath);

	if (vertexSource.empty() || fragmentSource.empty()) {
		return false;
	}

	const unsigned int vertexShader = Compile(GL_VERTEX_SHADER, vertexSource);
	if (vertexShader == 0) {
		return false;
	}

	const unsigned int fragmentShader = Compile(GL_FRAGMENT_SHADER, fragmentSource);
	if (fragmentShader == 0) {
		glDeleteShader(vertexShader);
		return false;
	}

	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertexShader);
	glAttachShader(m_programId, fragmentShader);
	glLinkProgram(m_programId);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int success = 0;
	glGetProgramiv(m_programId, GL_LINK_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(m_programId, 512, nullptr, infoLog);
		cerr << "Failed to link shader program:\n" << infoLog << "\n";

		Destroy();
		return false;
	}

	return true;
}

void Shader::SetInt(const string& name, int value) const {
	const int location = glGetUniformLocation(m_programId, name.c_str());

	if (location == -1) {
		cerr << "Uniform not found: " << name << "\n";
		return;
	}

	glUniform1i(location, value);
}

void Shader::Use() const {
	glUseProgram(m_programId);
}

void Shader::SetFloat4(const string& name, float x, float y, float z, float w) const {
	const int location = glGetUniformLocation(m_programId, name.c_str());

	if (location == -1) {
		cerr << "Uniform not found: " << name << "\n";
		return;
	}

	glUniform4f(location, x, y, z, w);
}

void Shader::SetMat4(const string& name, const glm::mat4& value) const {
	const int location = glGetUniformLocation(m_programId, name.c_str());

	if (location == -1) {
		cerr << "Uniform not found: " << name << "\n";
		return;
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::Destroy() {
	if (m_programId != 0) {
		glDeleteProgram(m_programId);
		m_programId = 0;
	}
}

unsigned int Shader::Compile(unsigned int type, const string& source)const {
	const unsigned int shader = glCreateShader(type);
	const char* sourcePtr = source.c_str();

	glShaderSource(shader, 1, &sourcePtr, nullptr);
	glCompileShader(shader);

	int success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		cerr << "Failed to compile shader:\n" << infoLog << "\n";

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

string Shader::ReadFile(const string& path) const {
	ifstream file(path);

	if (!file) {
		cerr << "Failed to open file: " << path << "\n";
		return {};
	}

	stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}
#pragma once

#include <glad/glad.h>
#include "Object.h"
#include "Color.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

namespace LEapsGL {
	const char* GetShaderTypeName(GLenum type);
	std::string ReadFile(const char* filePath);

	class ShaderProgram : Object{
	private:
		struct ShaderInfo {
			GLuint shaderType;
			GLuint shaderId;
		};

		GLuint id = 0;
		vector<ShaderInfo> registeredShader;

		unsigned int getShaderIndex(GLuint shaderType);

	public:		
		GLuint getID();
		void setID(GLuint id);
		void use();
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setFloat(const std::string& name, vector<float> v) const;
		void setColor(const std::string& name, const Color& v, const GLuint type = GL_RGBA) const;

		friend void swap(ShaderProgram& a, ShaderProgram& b) {
			using std::swap;
			swap(a.id, b.id);
			swap(a.registeredShader, b.registeredShader);
		}

		// Constructor
		ShaderProgram() : Object(), id(glCreateProgram()) {
		}
		ShaderProgram(GLuint _id) : Object(), id(_id) {
		}
		// Copy Constructor
		ShaderProgram(const ShaderProgram& other):Object() {
			this->id = other.id;
			registeredShader = other.registeredShader;
		}

		// Move constructor
		ShaderProgram(ShaderProgram&& other) noexcept: ShaderProgram(0) {
			swap(*this, other);
		}
		ShaderProgram& operator=(ShaderProgram other)  {
			swap(*this, other);
			return *this;
		}
		ShaderProgram& operator=(ShaderProgram&& other) noexcept{
			swap(*this, other);
			return *this;
		}

		virtual ~ShaderProgram() {
			DeleteShaderAll();
			glDeleteProgram(id);
		};
		ShaderProgram& CompileFromFile(GLuint shaderType, const char* path);
		ShaderProgram& CompileFromSourceCode(GLuint shaderType, const char* sourceCode);
		ShaderProgram& MakeProgram();
		ShaderProgram& DeleteShaderAll();
	};
};


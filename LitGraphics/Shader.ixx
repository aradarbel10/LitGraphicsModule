module;
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdarg>

export module Shader;

import Tuple;

namespace lgm {
	export class Shader {
	public:

		Shader(GLuint type, std::string_view path) {
			shader = glCreateShader(type);
			const char* source;

			if (loadFromFile(path, &source)) {
				glShaderSource(shader, 1, &source, NULL);
				glCompileShader(shader);

				delete source;
			}

			
		}

		~Shader() {
			glDeleteShader(shader);
		}

		GLuint get() const {
			return shader;
		}

	private:

		GLuint shader;

		bool loadFromFile(const std::string_view dir, const char** out) {
			std::ifstream file(dir.data(), std::ios::in);

			if (!file.is_open()) {
				std::cerr << "cannot open file at " << dir << '\n';
				return false;
			} else {
				std::string line;
				std::string* str = new std::string;

				while (std::getline(file, line)) {
					str->append(line + '\n');
				}
				file.close();
				*out = str->c_str();

				return true;
			}
		}
	};


	export class ShaderProgram {
	public:

		ShaderProgram() {
			program = glCreateProgram();
		}
		//ShaderProgram() = delete;

		ShaderProgram(const std::string_view vert, const std::string_view frag) {
			program = glCreateProgram();

			lgm::Shader vertexShader(GL_VERTEX_SHADER, vert);
			lgm::Shader fragmentShader(GL_FRAGMENT_SHADER, frag);

			*this << vertexShader.get() << fragmentShader.get();
			link();
		}

		ShaderProgram& operator<<(const GLuint shader) {
			glAttachShader(program, shader);
			return *this;
		}

		void link() {
			glLinkProgram(program);
		}

		GLuint get() const {
			return program;
		}

		void use() const {
			glUseProgram(program);
		}

		void setColorUniform(const lgm::color c) const {
			GLuint loc = glGetUniformLocation(program, "color");
			glUniform4f(loc, c.r, c.g, c.b, c.a);
		}

		~ShaderProgram() {
			glDeleteProgram(program);
		}

	private:

		GLuint program;

	};
}
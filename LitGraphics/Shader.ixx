module;
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdarg>

export module Shader;

namespace lgm {
	export class Shader {
	public:

		Shader(GLuint type, std::string_view path) {
			shader = glCreateShader(type);
			const char* source;

			loadFromFile(path, &source);

			glShaderSource(shader, 1, &source, NULL);
			glCompileShader(shader);

			delete source;
		}

		~Shader() {
			glDeleteShader(shader);
		}

		GLuint get() const {
			return shader;
		}

	private:

		GLuint shader;

		void loadFromFile(const std::string_view dir, const char** out) {
			std::ifstream file(dir.data(), std::ios::in);

			if (!file.is_open()) {
				std::cerr << "cannot open file at " << dir << '\n';
			} else {
				std::string line;
				std::string* str = new std::string;

				while (std::getline(file, line)) {
					str->append(line + '\n');
				}
				file.close();
				*out = str->c_str();
			}
		}

	};


	export class ShaderProgram {
	public:

		ShaderProgram() {
			program = glCreateProgram();
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

		~ShaderProgram() {
			glDeleteProgram(program);
		}

	private:

		GLuint program;

	};
}
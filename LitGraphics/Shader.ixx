module;
#include <glad/glad.h>

#include <string>
#include <vector>
#include <algorithm>

#include <fstream>

#include <iostream>
#include <format>

export module Shader;

import Tuple;

namespace lgm {
	export class Shader {
	public:

		Shader(GLuint type, std::string_view path) {
			const char* source;

			if (loadFromFile(path, &source)) {
				shader = glCreateShader(type);
				glShaderSource(shader, 1, &source, NULL);
				glCompileShader(shader);
				
				GLint success = 0;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

				if (success == GL_FALSE) {
					GLint maxLength = 0;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

					std::vector<GLchar> errorLog(maxLength);
					glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

					std::ranges::for_each(errorLog, [](GLchar c) { std::cout << c; });

					glDeleteShader(shader);
				}

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

			u_color = glGetUniformLocation(program, "color");
			u_tpos = glGetUniformLocation(program, "tpos");
			u_tscale = glGetUniformLocation(program, "tscale");
			u_trot = glGetUniformLocation(program, "trot");
			u_wsize = glGetUniformLocation(program, "wsize");
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

		void setColorUniform(const lgm::color& c) const {
			glUniform4f(u_color, c.r, c.g, c.b, c.a);
		}

		void setTranfsormUniform(const lgm::Transform& t) const {
			glUniform2f(u_tpos, t.position.x, t.position.y);
			glUniform2f(u_tscale, t.scale.x, t.scale.y);
			glUniform1f(u_trot, t.angle);
		}

		void setWinSizeUniform(const lgm::vector2f& s) const {
			glUniform2f(u_wsize, s.x, s.y);
		}

		~ShaderProgram() {
			glDeleteProgram(program);
		}

	private:

		GLuint program;
		GLuint u_color, u_tpos, u_tscale, u_trot, u_wsize;

	};
}
#pragma once

#include <glad/glad.h>

#include <string>
#include <vector>
#include <algorithm>

#include <fstream>

#include <iostream>
#include <format>

#include "Tuple.h"

namespace lgm {
	class Shader {
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


	class ShaderProgram {
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

			u_tex_tpos = glGetUniformLocation(program, "tex_tpos");
			u_tex_tscale = glGetUniformLocation(program, "tex_tscale");
			u_tex_trot = glGetUniformLocation(program, "tex_trot");
			u_has_texture = glGetUniformLocation(program, "has_texture");
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

		void setTextureTransformUniform(const lgm::Transform& t) const {
			glUniform2f(u_tex_tpos, t.position.x, t.position.y);
			glUniform2f(u_tex_tscale, t.scale.x, t.scale.y);
			glUniform1f(u_tex_trot, t.angle);
			glUniform1i(u_has_texture, true);
		}

		void unsetHasTextureUniform() const {
			glUniform1i(u_has_texture, false);
		}

		void setWinSizeUniform(const lgm::vector2f& s) const {
			glUniform2f(u_wsize, s.x, s.y);
		}

		~ShaderProgram() {
			glDeleteProgram(program);
		}

	private:

		GLuint program = 0;

		GLuint u_color = 0;
		GLuint u_wsize = 0;

		GLuint u_tpos = 0;
		GLuint u_tscale = 0;
		GLuint u_trot = 0;

		GLuint u_tex_tpos = 0;
		GLuint u_tex_tscale = 0;
		GLuint u_tex_trot = 0;
		GLuint u_has_texture = 0;

	};
}
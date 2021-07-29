module;
#include <glad/glad.h>

#include <span>
#include <vector>
#include <map>

#include <iostream>
#include <format>

export module VertexArray;

namespace lgm {
	
	struct VertexAttribFormat {
		VertexAttribFormat(GLint _size, GLenum _type, GLboolean _normalized, GLsizei _stride, const GLvoid* _pointer) 
			: size(_size), type(_type), normalized(_normalized), stride(_stride), pointer(_pointer) {};

		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		const GLvoid* pointer;
	};

	export template<typename T, GLuint B>
	class BufferObject {
	public:

		BufferObject() = default;

		BufferObject(std::span<T> vertices) {
			constructData(vertices.data(), vertices.size_bytes());
		}

		void constructData(T* data, size_t size) {
			glGenBuffers(1, &buffer);
			glBindBuffer(B, buffer);
			glBufferData(B, size, data, GL_STATIC_DRAW);
			glBindBuffer(B, 0);
		}

		GLuint get() const {
			return buffer;
		}

		void bind() const {
			glBindBuffer(B, buffer);
		}

		void unbind() const {
			glBindBuffer(B, 0);
		}

		/*size_t addAttrib(VertexAttribFormat&& attr) {
			attribs.push_back(attr);
			return attribs.size() - 1;
		}*/

		size_t setAttrib(GLuint index, VertexAttribFormat&& attr) {
			attribs[index] = attr;
			return index;
		}

		const std::map<int, VertexAttribFormat>& getAttribs() const {
			return attribs;
		}

		~BufferObject() {
			glDeleteBuffers(1, &buffer);
		}

	private:

		GLuint buffer;
		std::map<int, VertexAttribFormat> attribs;

	};

	export typedef BufferObject<GLfloat, GL_ARRAY_BUFFER> VBO;
	export typedef BufferObject<GLuint, GL_ELEMENT_ARRAY_BUFFER> IBO;

	export class VAO {
	public:

		VAO() {
			glGenVertexArrays(1, &vao);
		}

		template<typename T, GLuint B>
		void linkBuffer(const BufferObject<T, B>& vbo) {
			glBindVertexArray(vao);
			vbo.bind();

			for (auto const& [i, attr] : vbo.getAttribs()) {
				glVertexAttribPointer(i, attr.size, attr.type, attr.normalized, attr.stride, attr.pointer);
				glEnableVertexAttribArray(i);
			}

			glBindVertexArray(0);
			vbo.unbind();
		}

		void bind() const {
			glBindVertexArray(vao);
		}

		void unbind() const {
			glBindVertexArray(0);
		}

		~VAO() {
			glDeleteVertexArrays(1, &vao);
		}

	private:

		GLuint vao;

	};

}
module;

#include <glad/glad.h>
#include <stbi/stb_image.h>

#include <string_view>

export module Texture;

import Tuple;

namespace lgm {
	export class Texture {
	public:

		Texture(std::string_view path) {
			stbi_load(path.data(), &width, &height, &num_ch, 0);

			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			stbi_image_free(data);
		}

		lgm::vector2i inline size() {
			return { width, height };
		}

		~Texture() {
			glDeleteTextures(1, &tex);
		}

	private:

		unsigned char* data;
		int width = 0, height = 0, num_ch = 0;

		GLuint tex;

	};
}
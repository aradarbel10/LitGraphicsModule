module;

#include <glad/glad.h>
#include <numbers>
#include <numeric>
#include <span>

#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

#include <iostream>
#include <format>

export module Polygon;

import VertexArray;
import Tuple;
import Window;
import Shader;

namespace lgm {


	float det2(const vector2f& col1, const vector2f& col2) {
		return col1.x * col2.y - col1.y * col2.x;
	}

	bool pointInTriangle(const vector2f& P, const vector2f& A, const vector2f& B, const vector2f& C) {
		float d1 = det2(P - B, A - B),
			d2 = det2(P - C, B - C),
			d3 = det2(P - A, C - A);

		return (d1 < 0 && d2 < 0 && d3 < 0) || (d1 > 0 && d2 > 0 && d3 > 0);
	}

	float signedAngle(const vector2f& A, const vector2f& O, const vector2f& B) {
		return atan2f(det2(O - A, O - B), (O - A).x * (O - B).x + (O - A).y * (O - B).y);
	}

	export class Polygon {
	public:

		Polygon() {
			if (!shaderProgram) {
				shaderProgram = std::make_unique<lgm::ShaderProgram>("shaders/default_vertex.glsl", "shaders/default_fragment.glsl");
				//std::cout << "first polygon initialized!\n";
			}
		}

		Polygon& pushVertex(const lgm::vector2f& point) {
			vertices.push_back(point);
			return *this;
		}

		void setColor(const lgm::color& col) {
			color = col;
		}

		void setPosition(const lgm::vector2f& p) {
			position = p;
		}

		lgm::vector2f getPosition() const {
			return position;
		}

		void draw(lgm::ShaderProgram& sdr, bool drawMesh = false) const {
			sdr.use();
			vao.bind();
			sdr.setColorUniform(color);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, (vertices.size()) * 3, GL_UNSIGNED_INT, 0);

			if (drawMesh) {
				sdr.setColorUniform({ 1.0f, 0.0f, 0.0f, 1.0f });
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawElements(GL_TRIANGLES, (vertices.size()) * 3, GL_UNSIGNED_INT, 0);
			}
		}

		void draw(bool mesh = false) const {
			draw(*shaderProgram.get(), mesh);
		}

		virtual void triangulate() {
			//set up vectors used for triangulation
			std::vector<unsigned int> n; //maps the vertices of the polygon left while cutting ears, to the vertices of the actual polygon data
			n.resize(vertices.size());
			std::iota(n.begin(), n.end(), 0);

			std::vector<unsigned int> triangles; //will store the resulting ears in groups of 3

			unsigned int i = -1;
			while (n.size() >= 3) { //iterate through triangles
				i++;

				//calculate indices with offset, and warp then if they go out of range ("warped indices")
				unsigned int wi0 = i % n.size(), wi1 = (i + 1) % n.size(), wi2 = (i + 2) % n.size();

				//first check -- acute angle?
				float ang = signedAngle(vertices[n[wi0]], vertices[n[wi1]], vertices[n[wi2]]);
				if (ang <= 0 || ang >= std::numbers::pi) continue; //assumes vertices are in clockwise order

				//second check -- is ear?
				if ([&]()->bool {
					for (int j = 0; j < n.size() - 2; j++) {
						if (j - i >= 0 && j - i <= 2) continue; //point is part of the triangle, ignore it
						if (pointInTriangle(vertices[n[j]], vertices[n[wi0]], vertices[n[wi1]], vertices[n[wi2]])) return false; //point inside triangle
					}
					return true;
				}()) { //if-statemenet executed only if triangle doesn't contain any other vertices
					//an ear was found
					triangles.insert(triangles.end(), { n[wi0], n[wi1], n[wi2] });
					n.erase(n.begin() + i + 1); //remove ear from polygon

					i = -1; //start over from the first triangle
				}
			}

			//copy the vertex info into buffers
			vbo.constructData((float*)vertices.data(), vertices.size() * sizeof(float) * 2);
			ibo.constructData(triangles.data(), triangles.size() * sizeof(unsigned int));

			vbo.addAttrib({ 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0 });

			vao.linkBuffer(vbo);
			vao.linkBuffer(ibo);
		}

	private:

		// rendering
		lgm::VAO vao;
		lgm::VBO vbo;
		lgm::IBO ibo;
		
		static std::unique_ptr<lgm::ShaderProgram> shaderProgram;

		// shape
		lgm::vector2f position{ 0, 0 };
		std::vector<lgm::vector2f> vertices;

		lgm::color color;

	};

	std::unique_ptr<lgm::ShaderProgram> Polygon::shaderProgram;// = std::make_unique<lgm::ShaderProgram>("shaders/default_vertex.glsl", "shaders/default_fragment.glsl");

}
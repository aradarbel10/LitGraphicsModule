#pragma once

namespace lgm {

	template <typename T>
	struct vector2 {
		vector2(T x_, T y_) : x(x_), y(y_) { }

		T x, y;

		vector2<T> operator+(const vector2<T>& other) const {
			return { x + other.x, y + other.y };
		}

		vector2<T> operator-(const vector2<T>& other) const {
			return { x - other.x, y - other.y };
		}
	};
	typedef vector2<int> vector2i;
	typedef vector2<float> vector2f;
	typedef vector2<unsigned int> vector2u;

	template <typename T>
	struct vector3 {
		T x, y, z;
	};
	typedef vector3<int> vector3i;
	typedef vector3<float> vector3f;
	typedef vector3<unsigned int> vector3u;

	class Transform {
	public:

		Transform() = default;
		Transform(lgm::vector2f&& p, lgm::vector2f&& s, float a) : position(p), scale(s), angle(a) {} ;

		lgm::vector2f position{ 0, 0 };
		lgm::vector2f scale{ 1, 1 };
		float angle = 0;

	};

	struct color {
		color(float _r, float _g, float _b, float _a = 1.0f)
			: r(_r), g(_g), b(_b), a(_a) {};
		float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;

		color() = default;

		void normalize() {
			r /= 255.f;
			g /= 255.f;
			b /= 255.f;
		}
	};

}
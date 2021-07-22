export module Tuple;

export namespace lgm {

	template <typename T>
	struct vector2 {
		T x, y;
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

	struct color {
		color(float _r, float _g, float _b, float _a = 1.0f)
			: r(_r), g(_g), b(_b), a(_a) {};
		float r, g, b, a;

		void normalize() {
			r /= 255.f;
			g /= 255.f;
			b /= 255.f;
		}
	};

}
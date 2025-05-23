#pragma once


struct Color {
	union {
		struct { float r, g, b, a; };
		float data[4];
	};

	Color() = default;
	~Color() = default;

	Color(const float r, const float g, const float b, const float a = 1.0f);
	Color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a = 255);

	static const Color White;
	static const Color Black;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Purple;
	static const Color Cyan;
};
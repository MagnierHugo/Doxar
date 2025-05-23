#include "Render/Color.hpp"

#define INV_255 0.003921568627f


const Color Color::White	{ 1.0f, 1.0f, 1.0f };
const Color Color::Black	{  .0f,  .0f,  .0f };
const Color Color::Red		{ 1.0f,  .0f,  .0f };
const Color Color::Green	{  .0f, 1.0f,  .0f };
const Color Color::Blue		{  .0f,  .0f, 1.0f };
const Color Color::Yellow	{ 1.0f,  .0f, 1.0f };
const Color Color::Purple	{ 1.0f,  .0f, 1.0f };
const Color Color::Cyan		{  .0f, 1.0f, 1.0f };


Color::Color(float r, float g, float b, float a) {
	data[0] = r;
	data[1] = g;
	data[2] = b;
	data[3] = a;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	data[0] = r * INV_255;
	data[1] = g * INV_255;
	data[2] = b * INV_255;
	data[3] = a * INV_255;
}

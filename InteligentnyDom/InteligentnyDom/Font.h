#pragma once

#include <vector>
#include <string>

#include <GLFW\glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H



class Font
{
public:
	std::string Name;
	float Height;
	std::vector<int> GlyphWidth;

public:
	GLuint *Textures;
	GLuint List;

public:
	Font(std::string filename = "");
	~Font();

	float GetTextLength(const char* text);

	bool Load(std::string filename);
};
#include "Font.h"


using namespace std;


Font::Font(std::string filename) : GlyphWidth(128)
{
	if (filename != "")
	{
		if (!Load(filename)) printf("Loading texture failed!");
	}
	else
	{
		Name = "";
		Height = 0;
	}
}




Font::~Font()
{
	glDeleteLists(List, 128);
	glDeleteTextures(128, Textures);
	delete[] Textures;
}

float Font::GetTextLength(const char* text)
{
	float length = 0;
	size_t count = strlen(text);
	for (size_t i = 0; i < count; i++)
	{
		length += GlyphWidth[text[i]];
	}
	return length;
}

inline int next_p2(int a)
{
	int rval = 1;
	while (rval<a) rval <<= 1;
	return rval;
}

bool Font::Load(std::string path)
{
	//Allocate some memory to store the texture ids.
	Textures = new GLuint[128];

	//Create and initilize a freetype font library.
	FT_Library library;
	if (FT_Init_FreeType(&library)) printf("FT_Init_FreeType failed");

	//The object in which Freetype holds information on a given
	//font is called a "face".
	FT_Face face;

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
	if (FT_New_Face(library, path.c_str(), 0, &face)) printf("FT_New_Face failed (there is probably a problem with your font file)");

	//For some twisted reason, Freetype measures font size
	//in terms of 1/64ths of pixels.  Thus, to make a font
	//h pixels high, we need to request a size of h*64.
	//(h << 6 is just a prettier way of writting h*64)
	FT_Set_Char_Size(face, (int)(Height * 64), (int)(Height * 64), 96, 96);

	//Here we ask opengl to allocate resources for
	//all the textures and displays lists which we
	//are about to create.  
	List = glGenLists(128);
	glGenTextures(128, Textures);

	//This is where we actually create each of the fonts display lists.
	for (unsigned char i = 0; i<128; i++)
	{
		//Load the Glyph for our character.
		if (FT_Load_Glyph(face, FT_Get_Char_Index(face, i), FT_LOAD_DEFAULT))
			throw std::runtime_error("FT_Load_Glyph failed");

		//Move the face's glyph into a Glyph object.
		FT_Glyph glyph;
		if (FT_Get_Glyph(face->glyph, &glyph))
			throw std::runtime_error("FT_Get_Glyph failed");

		//Convert the glyph to a bitmap.
		FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

		//This reference will make accessing the bitmap easier
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;

		int width = next_p2(bitmap.width);
		int height = next_p2(bitmap.rows);

		GLubyte* expanded_data = new GLubyte[2 * width * height];

		for (int j = 0; j <height; j++) {
			for (int i = 0; i < width; i++){
				expanded_data[2 * (i + j*width)] = expanded_data[2 * (i + j*width) + 1] =
					(i >= bitmap.width || j >= bitmap.rows) ?
					0 : bitmap.buffer[i + bitmap.width*j];
			}
		}


		glBindTexture(GL_TEXTURE_2D, Textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

		delete[] expanded_data;

		glNewList(List + i, GL_COMPILE);

		glBindTexture(GL_TEXTURE_2D, Textures[i]);

		glPushMatrix();

		glTranslatef((float)bitmap_glyph->left, 0, 0);

		glTranslatef(0, (float)bitmap_glyph->top - bitmap.rows, 0);

		float x = (float)bitmap.width / (float)width;
		float y = (float)bitmap.rows / (float)height;

		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2i(0, bitmap.rows);
		glTexCoord2d(0, y); glVertex2i(0, 0);
		glTexCoord2d(x, y); glVertex2i(bitmap.width, 0);
		glTexCoord2d(x, 0); glVertex2i(bitmap.width, bitmap.rows);
		glEnd();
		glPopMatrix();
		glTranslatef((float)(face->glyph->advance.x >> 6), 0, 0);

		this->GlyphWidth[i] = (face->glyph->advance.x >> 6);

		glEndList();
	}

	FT_Done_Face(face);

	FT_Done_FreeType(library);

	return true;
}

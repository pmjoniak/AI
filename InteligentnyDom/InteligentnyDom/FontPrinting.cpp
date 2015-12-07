#include "FontPrinting.h"
#include <vector>

using namespace std;

void glPrint(Font* font, float x, float y, const char *format, ...)
{
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(viewport[0], viewport[2], viewport[1], viewport[3],-1,1);
	glPopAttrib();

	GLuint list = font->List;
	float h = font->Height / .63f; //We make the height about 1.5* that of

	char text[256];
	va_list args;

	if (format == nullptr)
	{
		*text = 0;
	}
	else
	{
		va_start(args, format);
		vsprintf_s(text, format, args);
		va_end(args);
	}

	//Here is some code to split the text that we have been
	//given into a set of lines.  
	//This could be made much neater by using
	//a regular expression library such as the one avliable from
	//boost.org (I've only done it out by hand to avoid complicating
	//this tutorial with unnecessary library dependencies).
	const char *start_line = text;
	vector<string> lines;
	const char *c;
	for (c = text; *c; c++) {
		if (*c == '\n') {
			string line;
			for (const char *n = start_line; n<c; n++) line.append(1, *n);
			lines.push_back(line);
			start_line = c + 1;
		}
	}
	if (start_line) {
		string line;
		for (const char *n = start_line; n<c; n++) line.append(1, *n);
		lines.push_back(line);
	}

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glListBase(list);

	float modelview_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	//This is where the text display actually happens.
	//For each line of text we reset the modelview matrix
	//so that the line's text will start in the correct position.
	//Notice that we need to reset the matrix, rather than just translating
	//down by h. This is because when each character is
	//draw it modifies the current matrix so that the next character
	//will be drawn immediatly after it.  
	for (int i = 0; i<(int)lines.size(); i++)
	{
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x, y - h*i, 0);
		glMultMatrixf(modelview_matrix);

		//The commented out raster position stuff can be useful if you need to
		//know the length of the text that you are creating.
		//If you decide to use it make sure to also uncomment the glBitmap command
		//in make_dlist().
		//glRasterPos2f(0,0);
		glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
		//float rpos[4];
		//glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
		//float len=x-rpos[0];

		glPopMatrix();
	}

	glPopAttrib();

	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

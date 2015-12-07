#include "Renderer.h"
#include <cmath>
#include "FontPrinting.h"

Renderer::Renderer(Symulation& symulation, int width, int height) : width{ width }, height{ height }, symulation{ symulation }
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(width, height, "Inteligentny Dom", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	//glewInit();

	glfwSetErrorCallback(_handleErrorCallback);
	glfwSetCharCallback(window, _handleCharCallback);
	glfwSetDropCallback(window, _handleDropCallback);
	glfwSetKeyCallback(window, _handleKeyCallback);
	glfwSetMouseButtonCallback(window, _handleMouseButtonCallback);
	glfwSetCursorPosCallback(window, _handleCursorPosCallback);

}


void Renderer::handleCharCallback(unsigned int codepoint)
{
	(void)codepoint;
}

void Renderer::handleDropCallback(int count, const char** names)
{
	(void)count;
	(void)names;
}

void Renderer::handleKeyCallback(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, 1);
	if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_Q)
		controller->setTempOut(controller->getTempOut() - 1);
	if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_W)
		controller->setTempOut(controller->getTempOut() + 1);
	if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_A)
		controller->setTd(controller->getTd() - 1);
	if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_S)
		controller->setTd(controller->getTd() + 1);

	if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_Z)
		controller->setTimeSpeed(controller->getTimeSpeed()*0.9);
	if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_X)
		controller->setTimeSpeed(controller->getTimeSpeed()*1.1);
	(void)key;
	(void)scancode;
	(void)action;
	(void)mods;
}

void Renderer::handleMouseButtonCallback(int button, int action, int mods)
{
	(void)mods;
	if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (cx > 355 && cx < 355 + 90 && cy > 205 && cy < 205 + 90)
			controller->forceRain(true);
		if (cx > 455 && cx < 455 + 90 && cy > 205 && cy < 205 + 90)
			controller->forceRain(false);
		if (cx > 355 && cx < 355 + 90 && cy > 305 && cy < 305 + 90)
			controller->forceSun(true);
		if (cx > 455 && cx < 455 + 90 && cy > 305 && cy < 305 + 90)
			controller->forceSun(false);
	}
	else if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT)
	{
	}
}

void Renderer::handleCursorPosCallback(double xpos, double ypos)
{
	cx = (int)xpos;
	cy = (int)ypos;
}

void Renderer::run()
{
	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	font.Height = 12;
	font.Load("DejaVuSans.ttf");
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// glFrustum(fNear * (headX - fFov * ratio),
		// 		  fNear * (headX + fFov * ratio),
		// 		  fNear * (headY - fFov),
		// 		  fNear * (headY + fFov),
		// 		  fNear, fFar);
		glOrtho(0, width, height, 0, -10, 10);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		drawDay();
		drawRain();
		drawSun();
		drawStove();
		drawAC();
		drawWindow();
		drawShutters();
		showTemperatures();
		drawButtons();

		controller->update(0.05);

		glfwSwapBuffers(window);
		glfwPollEvents();
		// wait(10);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	glfwDestroyWindow(window);
}


void Renderer::drawDay()
{
	glPushMatrix();
	glColor3f(0, 0, 0);
	box(48, 48, 204, 54);
	glTranslatef(0, 0, 0.01);
	glColor3f(1, 1, 1);
	box(50, 50, 200, 50);
	glTranslatef(0, 0, 0.01);
	glColor3f(0, 0, 0.6);
	float day = symulation.getDayPercentage();
	box(50, 50, 200 * (day > 0.5 ? 0.5 : day), 50);
	glColor3f(1, 0.8, 0);
	if (day > 0.5)
		box(150, 50, 200 * (day - 0.5), 50);
	glPopMatrix();
}

void Renderer::drawRain()
{
	if (!symulation.isRaining()) return;
	glPushMatrix();
	glColor3f(0, 0.5, 1);
	for (int i = 0; i < 10; i++)
	{
		float x = 60 + rand() % 80;
		float y = 110 + rand() % 80;
		float r = 6 + rand() % 6;
		circle(x, y, r);
	}
	glPopMatrix();
}

void Renderer::drawSun()
{
	if (!symulation.isSun()) return;
	glPushMatrix();
	glColor3f(1, 1, 0);
	circle(200, 150, 40);
	glTranslatef(200, 150, 0);
	glLineWidth(4);
	glBegin(GL_LINES);
	for (float a = 0; a <= 2 * 3.3; a += 0.7f)
	{
		glVertex2f(0, 0);
		glVertex2f(50 * std::cos(a), 50 * std::sin(a));
	}
	glEnd();
	glLineWidth(1);
	glPopMatrix();
}

void Renderer::drawStove()
{
	glPushMatrix();
	glColor3f(0, 0, 0);
	box(55, 205, 90, 90);
	glTranslatef(0, 0, 0.01);
	if (symulation.isHeating())
		glColor3f(1, 0, 0);
	else
		glColor3f(0, 0, 1);
	box(65, 215, 70, 70);
	glPopMatrix();
}

void Renderer::drawAC()
{
	glPushMatrix();
	glColor3f(0, 0, 0);
	box(155, 205, 90, 40);
	glTranslatef(0, 0, 0.01);
	glColor3f(1, 1, 1);
	box(165, 209, 70, 8);
	box(165, 221, 70, 8);
	box(165, 233, 70, 8);

	if (symulation.isCooling())
	{
		glColor3f(0, 1, 0);
		glLineWidth(4);
		glTranslatef(200, 285, 0);
		glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(-10, -10);

		glVertex2f(0, 0);
		glVertex2f(20, -15);
		glEnd();
	}
	else
	{
		glColor3f(1, 0, 0);
		glLineWidth(2);
		glTranslatef(200, 275, 0);
		glBegin(GL_LINES);
		glVertex2f(-10, -10);
		glVertex2f(10, 10);

		glVertex2f(10, -10);
		glVertex2f(-10, 10);
		glEnd();
	}
	glLineWidth(1);
	glPopMatrix();
}

void Renderer::drawWindow()
{
	glPushMatrix();
	glColor3f(0, 0, 0);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(70, 310);
	glVertex2f(70, 330);

	if (symulation.isWindowOpen())
	{
		glVertex2f(100, 333);
		glVertex2f(70, 390);
	}
	else
	{
		glVertex2f(70, 333);
		glVertex2f(70, 390);
	}
	glEnd();
	glLineWidth(1);
	glPopMatrix();
}

void Renderer::drawShutters()
{
	glPushMatrix();
	if (symulation.isShutterOpen())
		glLineWidth(1);
	else
		glLineWidth(8);

	for (int i = 0; i < 9; i++)
	{
		glBegin(GL_LINES);
			glVertex2f(160, 300+5+i*10);
			glVertex2f(240, 300 + 5 + i * 10);
		glEnd();
	}
	glLineWidth(1);
	glPopMatrix();
}

void Renderer::showTemperatures()
{
	glColor3f(0, 0, 0);
	glPrint(&font, 320, 460, "W domu %.2f[°C].", symulation.getInsideTemerature());
	glPrint(&font, 320, 440, "Zadana %.2f[°C]." , controller->getTd());
	glPrint(&font, 320, 420, "Na zwewnatrz %.2f[°C].", symulation.getOutsideTemerarure());
	glPrint(&font, 320, 400, "Zuzyta energia %.2f.", symulation.getEnergyConsumption());
}

void Renderer::drawButtons()
{
	glPushMatrix();
	glColor3f(0, 1, 0.5);
	box(355, 205, 90, 90);
	box(455, 205, 90, 90);
	box(355, 305, 90, 90);
	box(455, 305, 90, 90);
	glColor3f(0, 0, 0);
	glPrint(&font, 360, height - 250, "Rain On");
	glPrint(&font, 460, height - 250, "Rain Off");
	glPrint(&font, 360, height - 350, "Sun On");
	glPrint(&font, 460, height - 350, "Sun Off");


	glPopMatrix();
}

void Renderer::circle(float x, float y, float r)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0, 0);
		for (float a = 0; a <= 2 * 3.3; a += 0.1f)
			glVertex2f(r * std::cos(a), r * std::sin(a));
	glEnd();
	glPopMatrix();
}

void Renderer::box(float x, float y, float w, float h)
{
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x+w, y);
	glVertex2f(x+w, y+h);
	glVertex2f(x, y+h);
	glEnd();
}

void Renderer::setController(std::shared_ptr<Controller> controller)
{
	this->controller = controller;
}

void Renderer::wait(int frame_count)
{
	next_frame += frame_count * frame_duration(1);
	std::this_thread::sleep_until(next_frame);
}

//----------------------------------------------------------------------------

void Renderer::_handleErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error: %d '%s'\n", error, description);
}

void Renderer::_handleCharCallback(GLFWwindow* window, unsigned int codepoint)
{
	((Renderer*)glfwGetWindowUserPointer(window))->handleCharCallback(codepoint);
}

void Renderer::_handleDropCallback(GLFWwindow* window, int count, const char** names)
{
	((Renderer*)glfwGetWindowUserPointer(window))->handleDropCallback(count, names);
}

void Renderer::_handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	((Renderer*)glfwGetWindowUserPointer(window))->handleKeyCallback(key, scancode, action, mods);
}

void Renderer::_handleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	((Renderer*)glfwGetWindowUserPointer(window))->handleMouseButtonCallback(button, action, mods);
}

void Renderer::_handleCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	((Renderer*)glfwGetWindowUserPointer(window))->handleCursorPosCallback(xpos, ypos);
}
